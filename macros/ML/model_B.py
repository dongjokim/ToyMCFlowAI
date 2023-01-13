# build the model

from tensorflow.keras.utils import plot_model
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D
from tensorflow.keras import models, layers, utils, backend as K
def binary_step_activation(x):
    ##return 1 if x>0 else 0 
    return K.switch(x>0, tf.math.divide(x,x), tf.math.multiply(x,0))

# define metrics
def Recall(y_true, y_pred):
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
    recall = true_positives / (possible_positives + K.epsilon())
    return recall

def Precision(y_true, y_pred):
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
    precision = true_positives / (predicted_positives + K.epsilon())
    return precision

def F1(y_true, y_pred):
    precision = Precision(y_true, y_pred)
    recall = Recall(y_true, y_pred)
    return 2*((precision*recall)/(precision+recall+K.epsilon()))

def R2(y, y_hat):
    ss_res =  K.sum(K.square(y - y_hat)) 
    ss_tot = K.sum(K.square(y - K.mean(y))) 
    return ( 1 - ss_res/(ss_tot + K.epsilon()) )

def MyModel(n_features):
  
  model_cnn = Sequential(name="DeepNN", layers=[
      ### 
      Dense(name="h", input_dim=n_features,
                   units=int(round((n_features+1)/24)), #128
                   activation='relu'),  
      ### hidden layer 1
      Dense(name="h1", units=int(round((n_features+1)/12)), #256
                   activation='relu'),
      ### hidden layer 2
      Dense(name="h2", units=int(round((n_features+1)/12)), 
                   activation='relu'),
      ### hidden layer 3
      Dense(name="h3", units=int(round((n_features+1)/12)), 
                   activation='relu'),
      ### layer output
      #Dense(name="output", units=1, activation='sigmoid')
      #Dense(name="output", units=1, activation='relu')
      Dense(name="output", units=1, activation='relu')
  ])

  model_cnn.summary()
  return model_cnn
