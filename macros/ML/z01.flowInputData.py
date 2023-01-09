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

def make_image_event(df):
	allimages = []
	trueimages = []
	flowprop = []
	xtitle = "$\\eta$"
	ytitle = "$\\varphi (\\mathrm{rad})$"
	for i in range(0,1000):
		myevent = df.loc[df['event'] == i]
		histopt, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['pt'])
		histomass, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['mass'])
		histoeCM, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['eCM'])
		histov2, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['v_2'])
		flowprop.append([myevent['v_2'],myevent['v_3'],myevent['psi_2'],myevent['psi_3']])
		allimages.append(histopt)
		allimages.append(histomass)
		allimages.append(histoeCM)
		trueimages.append(histov2)
	return allimages, trueimages, np.array(flowprop)


if __name__ == "__main__":
	tree = uproot3.open("../../tree_toymcflowAI_bg0NUE0.root")['vTree']
	print(tree.keys())
	#tree.arrays(["phi", "eta", "pt"])
	df = tree.pandas.df()
	#print(df)
	outdir = 'images_out/'
	if not os.path.isdir(outdir): os.system('mkdir {}'.format(outdir))
	cwd = os.getcwd()
	
	allimages, trueimages,flowprop  = make_image_event(df)
	np.savez_compressed(outdir+'allimages.npz', allimages)
	np.savez_compressed(outdir+'trueimages.npz', trueimages)
	np.savez_compressed(outdir+'.npz', flowprop)


#PlotInputMLEvents(df,"pt")
#PlotInputMLEvents(df,"mass")
#PlotInputMLEvents(df,"eCM")
#PlotInputEvents(df)
