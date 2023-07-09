############importar modulos#############
from pyArduino import *

from tkinter import *
from PIL import Image, ImageTk
import cv2
import mediapipe as mp

import numpy as np
from math import degrees, acos, sqrt
import sys



def toggle():#funcion del boton
    btn.config(text=bntVar.get())


def onclossing():
    arduino.sendData([0,0,0,0])#para el robot
    root.quit()
    cap.release()
    print('desconetar camara')
    root.destroy()

def euclidean_distance(point1, point2):
    x1, y1= point1.ravel()
    x2, y2= point2.ravel()
    distance= sqrt((x2-x1)**2 +(y2-y1)**2)
    return distance

def palm_centroid(coordinates_list): #calcula el centoide de la palma de la mano
    coordinates= np.array(coordinates_list)
    centroid= np.mean(coordinates, axis=0)
    centroid= int(centroid[0]), int(centroid[1])
    return centroid


def callback():
    cap.open(url)
    ret, frame= cap.read()

    if ret: #si se obtiene la imagen de la camara

        height, width, _= frame.shape #tamaño de la imagen 
        frame_rgb= cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)#convertimos la imagen a rgb
        results= holistic.process(frame_rgb)
        dirrecion=0
        rotacion=0
        comentario=""

        #mano derecha
        if results.right_hand_landmarks is not None: #si se detecta la mano derecha
            coordinates_palm=[]#coordenadas de la palma de la mano
            coordinate_ft=[]#coordenadas de la punta de los dedos
            coordinate_fb=[]#coordenadas de la base de los datos

            mp_drawing.draw_landmarks(#dibula los puntos de la mano y las conexiones
            frame, results.right_hand_landmarks, mp_holistic.HAND_CONNECTIONS)

            #palma de la mano
            for index in palm_points:
                x= int(results.right_hand_landmarks.landmark[index].x*width)#obtiene las coordenas en x
                y= int(results.right_hand_landmarks.landmark[index].y*height)#obtiene las coordenas en y
                coordinates_palm.append([x,y])#añade las coordenadas

            #coordenada punta del dedo
            for index in fingertips_point:
                x= int(results.right_hand_landmarks.landmark[index].x*width)
                y= int(results.right_hand_landmarks.landmark[index].y*height)
                coordinate_ft.append([x,y])

            #coordenada base del dedo
            for index in finger_base_point:
                x= int(results.right_hand_landmarks.landmark[index].x*width)
                y= int(results.right_hand_landmarks.landmark[index].y*height)
                coordinate_fb.append([x,y])
            

            nx, ny= palm_centroid(coordinates_palm)
            cv2.circle(frame, (nx, ny), 3, (0,255,0), 3)#dibuja un circulo en la palma de la mano
            coordinate_centroid=np.array([nx, ny])#pasamos las coordenadas a un array de numpy
            coordinate_ft= np.array(coordinate_ft)
            coordinate_fb= np.array(coordinate_fb)

            ######distancia euclusiana entre puntos
            #d_centroid_ft= euclidean_distance(coordinate_centroid, coordinate_ft)
            d_centroid_ft=np.linalg.norm(coordinate_centroid- coordinate_ft, axis=1)
            #d_centroid_fb= euclidean_distance(coordinate_centroid, coordinate_fb)
            d_centroid_fb= np.linalg.norm(coordinate_centroid- coordinate_fb, axis=1)
            dif= d_centroid_ft- d_centroid_fb

            if dif[0]>0 and dif[1]>0:
                comentario="rotacion"
                rotacion=255
                dirrecion=0

            elif dif[0]>0:
                dirrecion= 0# si el dedo esta extirado, el robot avanza hacia adelante
                rotacion=0
                comentario="adelante"
            else:
                dirrecion=255# si el dedo esta encogido, el robot avanza hacia atras
                rotacion=0
                comentario="atras"

            


        #cuerpo (brazo)
        if results.pose_landmarks is not None:
            mp_drawing.draw_landmarks(
            frame, results.pose_landmarks, mp_holistic.POSE_CONNECTIONS)
                
            codox=int(results.pose_landmarks.landmark[14].x*width)#obtiene las coordenas en x
            codoy=int(results.pose_landmarks.landmark[14].y*height)#obtiene las coordenas en y
            muñecax=int(results.pose_landmarks.landmark[16].x*width)
            muñecay=int(results.pose_landmarks.landmark[16].y*height)
            hombrox=int(results.pose_landmarks.landmark[12].x*width)
            hombroy=int(results.pose_landmarks.landmark[12].y*height)

            p1=np.array([muñecax,muñecay])#añade un array de numpy las coordenadas (x,y)
            p2=np.array([codox,codoy])
            p3=np.array([hombrox,hombroy])

            
            l1= euclidean_distance(p2,  p3)#encontramos la distancia euclusiana entre los puntos
            l2= euclidean_distance(p1,  p3)
            l3= euclidean_distance(p1,  p2)

            angulo= degrees(acos((l1**2 + l3**2 - l2**2)/(2 * l1 * l3)))#teorema del coseno y lo pasamos a grados
            salida_codo= int(((angulo-50)*255)/110)#una regla de 3 para que empiece en  5oº y termine en 160º

            if salida_codo>255:#limites de la salida codo
                salida_codo=255
            if salida_codo<0:
                salida_codo=0


            cv2.line(frame,(muñecax,muñecay), (codox,codoy), (255,255,0),10)#dibuja las lineas del brazo
            cv2.line(frame, (codox,codoy), (hombrox,hombroy), (255,255,0),10)
            #cv2.line(frame, (hombrox,hombroy), (muñecax,muñecay), (255,255,0), 4)
            cv2.circle(frame, (codox, codoy), 3, (255,0,0), 3)#dibuja un circulo en el codo
            cv2.circle(frame, (muñecax, muñecay), 3, (255,0,0), 3)#dibuja un circulo en el muñeca
            cv2.circle(frame, (hombrox, hombroy), 3, (255,0,0), 3)#dibuja un circulo en el hombro

            progress_bar=int((salida_codo*360)/255) #regla de 3 para un diagrama de bara ciruclar
            cv2.putText(frame, str(int(angulo))+", "+comentario, (450,30), 1, 1.5, (128, 0, 250), 2)#escribir en la pantalla
            cv2.ellipse(frame, (40,40), (30, 30), 0, 0, progress_bar, (0, 255, 0), -1)#diagrama de barras


        if bntVar.get()== 'pause':
            arduino.sendData([salida_codo,dirrecion,0,0])# cuando se pulsa strar

        else:
            arduino.sendData([0,0,0,0])#cuando se pulsa stop se envia 0 a los movimientos del robot 


        img=cv2.cvtColor(frame,cv2.COLOR_BGR2RGB)
        img= Image.fromarray(img)
        img.thumbnail((640,480))
        tKimage= ImageTk.PhotoImage(img)
        label.configure(image=tKimage)
        label.image= tKimage
        root.after(1,callback)

    else:
        onclossing()




##########camara ip#############
url= 'http://192.168.0.3:8080/shot.jpg'
cap= cv2.VideoCapture(url)

if cap.isOpened():
    print("camara ip inicializada")

else:
    sys.exit("camara ip desconectada")

########comunicacion serie#############
arduino = serialArduino('COM3', 115200)
#arduino.sendData([acelacion,direccion,izquierda,derecha, rotacion])

##########objetos de mediapipe#######
mp_drawing= mp.solutions.drawing_utils
mp_holistic= mp.solutions.holistic

#######puntos del cuerpo########### 
palm_points= [0, 1, 2, 5, 9, 13, 17]#puntos de la palma de la mano
fingertips_point=[8,20]
finger_base_point=[6,18]

########diseño del hmi################
root = Tk()
root.protocol("WM_DELETE_WINDOW",onclossing)
root.title("control con el cuerpo")

label= Label(root)
label.grid(row=0)

bntVar= StringVar(root, 'start')
btn= Checkbutton(root, text=bntVar.get(), width=12, variable=bntVar,
                 offvalue='start', onvalue='pause', indicatoron=False,
                 command=toggle)
btn.grid(row=6, padx=20, pady=20)

holistic= mp_holistic.Holistic(
    static_image_mode=False,
    model_complexity= 1)
callback()

root.mainloop()