# build the model

from tensorflow.keras.utils import plot_model
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D

def MyModel(n_features):
  
  model_cnn = Sequential(name="DeepNN", layers=[
      ### hidden layer 1
      Dense(name="h1", input_dim=n_features,
                   units=int(round((n_features+1)/2)), 
                   activation='relu'),
      Dropout(name="drop1", rate=0.2),
      
      ### hidden layer 2
      Dense(name="h2", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      Dropout(name="drop2", rate=0.2),

      ### hidden layer 3
      Dense(name="h3", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      Dropout(name="drop3", rate=0.2),

        ### hidden layer 4
      Dense(name="h4", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      Dropout(name="drop4", rate=0.2),

      ### layer output
      Dense(name="output", units=1, activation='sigmoid')
  ])

  model_cnn.summary()
  return model_cnn
