# build the model

from tensorflow.keras.utils import plot_model
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D

def MyModel(n_features):
  
  model_cnn = models.Sequential(name="DeepNN", layers=[
      ### hidden layer 1
      layers.Dense(name="h1", input_dim=n_features,
                   units=int(round((n_features+1)/2)), 
                   activation='relu'),
      layers.Dropout(name="drop1", rate=0.2),
      
      ### hidden layer 2
      layers.Dense(name="h2", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      layers.Dropout(name="drop2", rate=0.2),

      ### hidden layer 3
      layers.Dense(name="h3", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      layers.Dropout(name="drop3", rate=0.2),

        ### hidden layer 4
      layers.Dense(name="h4", units=int(round((n_features+1)/4)), 
                   activation='relu'),
      layers.Dropout(name="drop4", rate=0.2),

      ### layer output
      layers.Dense(name="output", units=1, activation='sigmoid')
  ])

  model_cnn.summary()
  return model_cnn
