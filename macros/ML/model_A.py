from tensorflow.keras.utils import plot_model
import keras
from keras.models import Sequential
from keras.layers import Dense, Flatten, Dropout, Activation, Conv2D, MaxPooling2D

def MyModel(x_train, y_train):
	model0 = Sequential()
	model0.add(Flatten(input_shape=(32,32,1)))  # Images are a 3D matrix, we have to flatten them to be 1D
	model0.add(Dense(2, kernel_initializer='normal', activation='softmax'))

	# Compile model
	model0.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

	history_logi = model0.fit(x_train, y_train, validation_split=0.2, epochs=40, batch_size=100, shuffle=True, verbose=0)
	model1 = Sequential()
	model1.add(Flatten(input_shape=(32, 32, 1)))  # Images are a 3D matrix, we have to flatten them to be 1D
	model1.add(Dense(100, kernel_initializer='normal', activation='tanh'))
	model1.add(Dropout(0.5)) # drop a unit with  50% probability.

	model1.add(Dense(100, kernel_initializer='orthogonal',activation='tanh'))
	model1.add(Dropout(0.5)) # drop a unit with  50% probability.

	model1.add(Dense(100, kernel_initializer='orthogonal',activation='tanh'))
	# model.add(Activation('sigmoid'))
	model1.add(Dense(2, kernel_initializer='normal', activation='softmax')) # last layer, this has a softmax to do the classification

	model1.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

	history_mlp = model1.fit(x_train, y_train, validation_split=0.2, epochs=40, batch_size=100, shuffle=True, verbose=1)
	model_cnn = Sequential()
	model_cnn.add(Conv2D(32, (3, 3), input_shape=(32, 32, 1), activation='relu'))
	model_cnn.add(Conv2D(32, (3, 3), activation='relu'))
	model_cnn.add(MaxPooling2D(pool_size=(2, 2)))
	model_cnn.add(Dropout(0.25))

	model_cnn.add(Conv2D(64, (3, 3), padding='same', activation='relu'))
	model_cnn.add(Conv2D(64, (3, 3), padding='same', activation='relu'))
	model_cnn.add(MaxPooling2D(pool_size=(2, 2)))
	model_cnn.add(Dropout(0.25))

	model_cnn.add(Flatten())
	model_cnn.add(Dense(300, activation='relu'))
	model_cnn.add(Dropout(0.5))
	model_cnn.add(Dense(2, activation='softmax'))

	model_cnn.summary()

	return model0, model1, history_mlp,model_cnn