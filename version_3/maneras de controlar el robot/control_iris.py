############importar modulos#############
from pyArduino import *

from tkinter import *
from PIL import Image, ImageTk
import cv2
import mediapipe as mp

import numpy as np
import math
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
    distance= math.sqrt((x2-x1)**2 +(y2-y1)**2)
    return distance

def iris_positionx(iris_center, right_point, left_point):
    center_to_right_dist= euclidean_distance(iris_center, right_point)
    total_dist= euclidean_distance(right_point, left_point)
    ratio= center_to_right_dist/total_dist
    iris_position=""
    if ratio <= 0.42:
        iris_position= "derecha"
        
    elif ratio > 0.42 and ratio <=0.60:
        iris_position= "centro"
    else:
        iris_position= "izquierda"
    return iris_position, ratio

def iris_positiony(iris_center, hight_point, low_point):
    center_to_hight_dist= euclidean_distance(iris_center, hight_point)
    total_dist= euclidean_distance(hight_point, low_point)
    ratio= center_to_hight_dist/total_dist
    iris_position=""
    if ratio <0.53:
        iris_position="arriba"
    elif ratio > 0.53 and ratio < 0.65:
        iris_position="centro"
    else:
        iris_position ="abajo"
    return iris_position, ratio

def callback():
    cap.open(url)
    ret, frame= cap.read()
    derecha=0
    izquierda=0
    arriba=0

    if ret:
        height, width,_= frame.shape #tamaño de la imagen
        frame_rgb= cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)#convertimos la imagen a rgb 
        results= face_mesh.process(frame_rgb)
        if results.multi_face_landmarks:
            mesh_points=np.array([
                    np.multiply([p.x, p.y], [width, height]).astype(int) 
                    for p in results.multi_face_landmarks[0].landmark
                ])
            (r_cx, r_cy), r_radius= cv2.minEnclosingCircle(mesh_points[RIGHT_IRIS])

            centre_right= np.array([r_cx, r_cy], dtype=np.int32)

            cv2.circle(frame, centre_right, int(r_radius), (255, 0, 255), 1, cv2.LINE_AA)

            cv2.circle(frame, mesh_points[R_H_RIGHT][0], 3, (255, 255, 255), -1, cv2.LINE_AA)
            cv2.circle(frame, mesh_points[R_H_LEFT][0], 3, (0, 255, 255), -1, cv2.LINE_AA)

            #cv2.circle(frame, mesh_points[R_H_HIGT][0], 3, (255, 0, 0), -1, cv2.LINE_AA)
            #cv2.circle(frame, mesh_points[R_H_LOW][0], 3, (0, 255, 0), -1, cv2.LINE_AA)

            iris_posx, ratio= iris_positionx(centre_right, mesh_points[R_H_RIGHT], mesh_points[R_H_LEFT][0])
            #iris_posy, ratio= iris_positiony(centre_right, mesh_points[R_H_HIGT], mesh_points[R_H_LOW][0])
            if iris_posx == "derecha":
                derecha=255
                izquierda=0
            elif iris_posx == "izquierda":
                izquierda=255
                derecha=0
            else:
                derecha=0
                izquierda=0
            
            '''iris_posy, ratioy= iris_positiony(centre_right, mesh_points[R_H_HIGT], mesh_points[R_H_LOW][0])
            if iris_posy== "arriba":
                arriba=255
            else:
                arriba=0'''
            
            cv2.putText(frame, f"{iris_posx}, {ratio:.2f}", (30, 30), 1, 1.5, (128, 0, 250), 2)

        if bntVar.get()== 'pause':
            arduino.sendData([0,0,izquierda,derecha])# cuando se pulsa strar

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
#arduino.sendData([acelacion,direccion,izquierda,derecha,rotacion])

##########objetos de mediapipe#######
mp_drawing= mp.solutions.drawing_utils
mp_face_mesh= mp.solutions.face_mesh

##########puntos del ojo###########
RIGHT_IRIS = [469, 470, 471, 472]
R_H_RIGHT = [133]  
R_H_LEFT = [33]   
R_H_HIGT = [27]  
R_H_LOW = [23]  

########diseño del hmi################
root = Tk()
root.protocol("WM_DELETE_WINDOW",onclossing)
root.title("control con el iris")

label= Label(root)
label.grid(row=0)

bntVar= StringVar(root, 'start')
btn= Checkbutton(root, text=bntVar.get(), width=12, variable=bntVar,
                 offvalue='start', onvalue='pause', indicatoron=False,
                 command=toggle)
btn.grid(row=6, padx=20, pady=20)

face_mesh= mp_face_mesh.FaceMesh(
    max_num_faces=1,
    refine_landmarks=True)
callback()

root.mainloop()
