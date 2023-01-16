import sys
import os
import uproot3
import ROOT
import pandas as pd
import numpy as np
import glob
#pd.options.plotting.backend = "plotly"
#import plotly.express as px
#from plotly.subplots import make_subplots
#import plotly.graph_objs as go
import matplotlib.pyplot as plt
#import seaborn as sns

from pathlib import Path


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
		flowinfo = np.array([myevent['v_2'].iloc[0],myevent['v_3'].iloc[0],myevent['psi_2'].iloc[0],myevent['psi_3'].iloc[0]])
		#print(flowinfo)
		flowprop.append(flowinfo) #1
		allimages.append(histopt)
		allimages.append(histomass)
		allimages.append(histoeCM)
		trueimages.append(histov2) # not needed!
	allimages = np.stack(allimages) #We want a 3D np array (n_events, xpixels, ypixels)
	print(np.asarray(allimages).shape,np.asarray(trueimages).shape,np.asarray(flowprop).shape)
	return allimages, trueimages, np.array(flowprop)


if __name__ == "__main__":
	#TODO: IMPLEMENT DATA STREAMER FOR THE TRAINING PROCESS.
	outdir = 'images_out/'
	try:
		os.mkdir(outdir);
	except FileExistsError:
		pass;

	for fn in glob.glob("../../outputs/*.root")[:2]:
		tree = uproot3.open(fn)['vTree']
		df = tree.pandas.df();

		allimages,trueimages,flowprop = make_image_event(df)

		nn = Path(fn).stem;
		#np.savez_compressed(outdir+'allimages_{}.npz'.format(nn), allimages)
		#np.savez_compressed(outdir+'trueimages_{}.npz'.format(nn), trueimages)
		#np.savez_compressed(outdir+'flowprop_{}.npz'.format(nn), flowprop)
		np.savez_compressed(outdir+'images_{}.npz'.format(nn),allimages,flowprop);

		print("Loaded {}...".format(fn));
	
	print(df.head());
	print("Rows:",len(df.index));


#PlotInputMLEvents(df,"pt")
#PlotInputMLEvents(df,"mass")
#PlotInputMLEvents(df,"eCM")
#PlotInputEvents(df)

