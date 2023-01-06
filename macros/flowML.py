import sys, os
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

def PlotInputMLEvents(df,weight):
	xtitle = "$\\eta$"
	ytitle = "$\\varphi (\\mathrm{rad})$"
	for i in range(0,10):
		myevent = df.loc[df['event'] == i]
		flights = myevent.pivot("eta", "phi", weight)
		fig = px.density_heatmap(myevent, x="eta", y="phi", z=weight, nbinsx=32, nbinsy=32, color_continuous_scale="Viridis")
		fig.update_layout(
			#title="Plot Title",
			xaxis_title=xtitle,
			yaxis_title=ytitle,
			legend_title="ToyMC",
			font=dict(
				family="Courier New, monospace",size=18,color="RebeccaPurple")
			)
		fig.show()
		fig.write_image	("figs/figML_{}_evt{}.pdf".format(weight,i))  

def make_image_event(df,weight):
	out=[]
	xtitle = "$\\eta$"
	ytitle = "$\\varphi (\\mathrm{rad})$"
	for i in range(0,10):
		myevent = df.loc[df['event'] == i]
		histo, xedges, yedges = np.histogram2d(myevent['eta'].to_numpy(), myevent['phi'].to_numpy(),bins=(32,32),weights=myevent['pt'].to_numpy())
		### append to output (transpose to have eta=x, phi=y)
		out.append(histo.T)
		#print(histo)
		#plt.imshow(image)
		#plt.show()
		#fig.write_image	("figs/figML_{}_evt{}.pdf".format(weight,i))     
	return out, (xedges, yedges)


if __name__ == "__main__":
	tree = uproot3.open("../tree_toymcflowAI_bg0NUE0.root")['vTree']
	print(tree.keys())
	#tree.arrays(["phi", "eta", "pt"])
	df = tree.pandas.df()
	#print(df)
	outdir = 'images_out/'
	if not os.path.isdir(outdir): os.system('mkdir {}'.format(outdir))
	cwd = os.getcwd()
	allimages = []
	hhpt, _ = make_image_event(df,"pt")
	hhmass, _ = make_image_event(df,"mass")
	hheCM, _ = make_image_event(df,"eCM")
	allimages.append(hhpt)
	allimages.append(hhmass)
	allimages.append(hheCM)
	np.savez_compressed(outdir+'allimages.npz', allimages)


#PlotInputMLEvents(df,"pt")
#PlotInputMLEvents(df,"mass")
#PlotInputMLEvents(df,"eCM")
#PlotInputEvents(df)