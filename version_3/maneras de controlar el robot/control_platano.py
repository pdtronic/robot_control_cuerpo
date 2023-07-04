############importar modulos#############
from pyArduino import *
from tkinter import *
from PIL import Image, ImageTk

import cv2
import numpy as np
import sys

def toggle():#funcion del boton
    btn.config(text=bntVar.get())

def onclossing():
    arduino.sendData([0,0,0,0])
    #print("lower=np.array(["+str(hMin.get())+","+str(sMin.get())+","+str(vMin.get())+"])")
    #print("upper=np.array(["+str(hMax.get())+","+str(sMax.get())+","+str(vMax.get())+"])")
    root.quit()
    cap.release()
    print("camara ip desconectada")
    root.destroy()

def hsvValue(int):
    hMin.set(slider1.get())
    hMax.set(slider2.get())
    sMin.set(slider3.get())
    sMax.set(slider4.get())
    vMin.set(slider5.get())
    vMax.set(slider6.get())

def objectdetection(rawimage):
    kernel= np.ones((5,5))#filtro
    isObject= False #verdadero si se encuentra el objeto
    cx, cy= 0,0  #centroide (x), centroide(y)
    height, angle=0, 0#ancho y angulo de rotacion

    minArea= 500 # area minima para considerar que es un objeto

    ###############procesamiento de la imagen#################
    hsv= cv2.cvtColor(rawimage, cv2.COLOR_BGR2HSV) #convertimos la imagen a hsv
    #se crea un array con las posiciones minimas y maximas capturadas por los sliders
    lower= np.array([hMin.get(), sMin.get(), vMin.get()])
    upper= np.array([hMax.get(), sMax.get(), vMax.get()])

    #deteccion de colores
    mask= cv2.inRange(hsv, lower, upper)
    mask= cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)

    ############segmentacion##################
    contours,_= cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)#busca los contornos cerrados

    for cnt in contours:
        #calculamos el centro a partir de cada momento
        momentos= cv2.moments(cnt)
        area= momentos['m00']
        if area>minArea:
            (x, y), (width, height), angle = cv2.minAreaRect(cnt)#para detectar el angulo
            if width> height:
                angle+=90
            new_cnt= cv2.convexHull(cnt)#suaviza los contornos
            cv2.drawContours(rawimage,[new_cnt],0,(255,0,0), 3)#dibuja el contorno
            cx= int(momentos['m10']/momentos['m00'])#coordenada del centro del objeto detectado
            cy= int(momentos['m01']/momentos['m00'])
            isObject= True
    return isObject, mask, rawimage, cx, cy, height, angle



def callback():
    cap.open(url)
    ret, frame= cap.read()
    
    if ret:
        isObject, mask, rawimage, cx, cy, height, angle= objectdetection(frame)
        cv2.circle(frame, (cx, cy), 10, (0,0,255), -1)#dibuja un circulo en el centro

        distancia=int(height-250)
        acelerador= int((distancia*255)/150)
   
        if acelerador>255:
            acelerador=255
        if acelerador<0:
            acelerador=0

        angle=int(angle)#angulo de inclinacion
        derecha=0
        if angle<80:#si el angulo es mas pequeño que 80º
            derecha=(-255/50)*(angle-40)+255#gira a derecha porporcionalmente
            if derecha>255:
                derecha=255
            if angle==0:
                derecha=0
            derecha=int(derecha)

        izquierda=0
        if angle>110:#si el angulo es superior a 110º
            izquierda=((angle-110)*255)/50#gira a la izquierda proporcionalmente
            if izquierda<0:
                izquierda=0
            if izquierda>255:
                izquierda=255
            izquierda=int(izquierda)
            
        
            
        velocidad2.set("angulo:"+str(int(angle))+" ,derecha:"+str(derecha)+" ,izquieda:"
                       +str(izquierda)+" ,aceleracion:"+str(acelerador)+", "+str(distancia))#informacion que se muestra en el hmi
        
        if bntVar.get()== 'pause':
            arduino.sendData([acelerador,0,derecha,izquierda])# cuando se pulsa strar

        else:
            arduino.sendData([0,0,0,0])#cuando se pulsa stop se envia 0 a los movimientos del robot 
        
        #mostrar imagen el hmi
        img1= cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        img1= Image.fromarray(img1)
        img1.thumbnail((400,400))
        tkimage1= ImageTk.PhotoImage(img1)
        label1.configure(image=tkimage1)
        label1.image= tkimage1

        img2= Image.fromarray(mask)
        img2.thumbnail((400,400))
        tkimage2= ImageTk.PhotoImage(img2)
        label2.configure(image=tkimage2)
        label2.image= tkimage2

        root.after(1, callback)
    else:
        onclossing()


##########camara ip#############
url= 'http://192.168.0.3:8080/shot.jpg'
cap= cv2.VideoCapture(url)

if cap.isOpened():
    print("camara inicializada")
else:
    sys.exit("camara ip desconectada")

########comunicacion serie#############
arduino = serialArduino('COM3', 115200)
#arduino.sendData([acelacion,direccion,izquierda,derecha,rotacion])

########diseño del hmi################

root= Tk()
root.protocol("WM_DELETE_WINDOW", onclossing)
root.title("vision artificial")

label1= Label(root)
label1.grid(padx=20, pady=20)

label2=Label(root)
label2.grid(row=0, column=1, padx=20, pady=20)

velocidad = DoubleVar(root,0)
velocidad2= StringVar(root, "velocidad:0")
labelv= Label(root, textvariable=velocidad2)
labelv.grid(row=1, padx=20, pady=20)

hMin= IntVar()
hMax= IntVar()
sMin= IntVar()
sMax= IntVar()
vMin= IntVar()
vMax= IntVar()

slider1= Scale(root, label='hue min', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider1.grid( row=3, column=0)
slider2= Scale(root, label='hue max', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider2.grid(row=3, column=1)
slider3= Scale(root, label='saturacion min', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider3.grid(row=4, column=0)
slider4= Scale(root, label='saturacion max', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider4.grid(row=4, column=1)
slider5= Scale(root, label='value min', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider5.grid(row=5, column=0)
slider6= Scale(root, label='value max', from_=0, to=255, orient=HORIZONTAL, command=hsvValue, length=400) #Creamos un dial para recoger datos numericos
slider6.grid(row=5, column=1)

slider1.set(17)
slider2.set(38)
slider3.set(100)
slider4.set(255)
slider5.set(100)
slider6.set(255)

bntVar= StringVar(root, 'start')
btn= Checkbutton(root, text=bntVar.get(), width=12, variable=bntVar,
                 offvalue='start', onvalue='pause', indicatoron=False,
                 command=toggle)
btn.grid(row=6, padx=20, pady=20)

root.after(1, callback)
root.mainloop()