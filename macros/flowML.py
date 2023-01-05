import uproot3
import ROOT
import pandas as pd
import numpy as np
pd.options.plotting.backend = "plotly"
import plotly.express as px
from plotly.subplots import make_subplots
import plotly.graph_objs as go
import matplotlib.pyplot as plt
import seaborn as sns
# define the function
import tensorflow as tf
from tensorflow.keras import models, layers, utils, backend as K
import shap
from tensorflow.keras.utils import plot_model
df = px.data.tips()

tree = uproot3.open("../tree_toymcflowAI_bg0NUE0.root")['vTree']
print(tree.keys())
#tree.arrays(["phi", "eta", "pt"])
df = tree.pandas.df()
#print(df)

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



# build the model
n_features = 4
model = models.Sequential(name="DeepNN", layers=[
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

model.summary()
#plot_model(model, to_file='model_plot.png', show_shapes=True, show_layer_names=True)

# compile the neural network
#model.compile(optimizer='adam', loss='binary_crossentropy', 
#              metrics=['accuracy',F1])

#df_train =  df[df['icent'] == 1]

#X = df_train[['event','phi','eta','pt']].values
#y = df_train[['v_2']].values
# train/validation
#training = model.fit(X, y, batch_size=32, epochs=100, shuffle=True, verbose=1, validation_split=0.3)
#predictions = (model.predict(X) > 0.0).astype(int)
# summarize the first 5 cases
#for i in range(20):
# print('%s => %f (expected %f)' % (X[i].tolist(), predictions[i], y[i]))

def PlotInputEvents(df):
	xtitle = "$\\eta$"
	ytitle = "$\\varphi (\\mathrm{rad})$"
	for i in range(0,10):
		myevent = df.loc[df['event'] == i]
		hphi = myevent['phi'].to_numpy()
		N = hphi.size
		t = np.arange(N)
		f = np.cos(2*np.pi * t/N)
		ft = np.fft.fft(f)
		#print(ft)
		#ic = myevent['icent']
		#print(i, hphi)
		#fig = px.histogram(myevent, x="phi", nbins=200)
		fig = plt.scatter(x=myevent['eta'], y=myevent['phi'],s=5*myevent['pt'],vmin=0, vmax=10)
		ax = plt.gca()
		ax.set_xlabel(xtitle)
		ax.set_ylabel(ytitle)
		plt.show()
		ax.figure.savefig("figs/fig_evt{}.pdf".format(i))  

def PlotInputMLEvents(df):
	xtitle = "$\\eta$"
	ytitle = "$\\varphi (\\mathrm{rad})$"
	for i in range(0,10):
		myevent = df.loc[df['event'] == i]
		flights = myevent.pivot("eta", "phi", "E")
		fig = px.density_heatmap(myevent, x="eta", y="phi", z="E", nbinsx=25, nbinsy=25, color_continuous_scale="Viridis")
		fig.update_layout(
			#title="Plot Title",
			xaxis_title=xtitle,
			yaxis_title=ytitle,
			legend_title="ToyMC",
			font=dict(
				family="Courier New, monospace",size=18,color="RebeccaPurple")
			)
		fig.show()
		fig.write_image("figs/figML_evt{}.pdf".format(i))  

PlotInputMLEvents(df)
#PlotInputEvents(df)