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

from multiprocessing import Pool
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
	N = np.max(df['event'])+1;
	for i in range(0,N):
		myevent = df.loc[df['event'] == i]
		histopt, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['pt'])
		histomass, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['mass'])
		histoeCM, xedges, yedges = np.histogram2d(myevent['eta'], myevent['phi'],bins=(32,32),weights=myevent['eCM'])
		flowinfo = np.array([myevent['v_2'].iloc[0],myevent['v_3'].iloc[0],myevent['psi_2'].iloc[0],myevent['psi_3'].iloc[0]])

		allimages.append(np.array([histopt,histomass,histoeCM]));
		flowprop.append(flowinfo) #1
	allimages = np.stack(allimages) #We want a 3D np array (n_events, xpixels, ypixels)
	print(np.asarray(allimages).shape,np.asarray(flowprop).shape)
	return allimages,np.array(flowprop)

def process(fn):
	print("Loading {}...".format(fn));

	tree = uproot3.open(fn)['vTree']
	df = tree.pandas.df();

	allimages,flowprop = make_image_event(df)

	nn = Path(fn).stem;
	np.savez_compressed(outdir+'images_{}.npz'.format(nn),allimages,flowprop);

	print("Done: {}.".format(fn));

if __name__ == "__main__":
	outdir = 'images_out/'
	try:
		os.mkdir(outdir);
	except FileExistsError:
		pass;

	with Pool(processes=20) as p:
		for fn in glob.glob("../../outputs/*.root"):
			p.apply_async(process,args=(fn,));
		p.close();
		p.join();
	#for fn in glob.glob("../../outputs/*.root")[:1]: #<---testing
	#	process(fn);
	
	#print(df.head());
	#print("Rows:",len(df.index));

#PlotInputMLEvents(df,"pt")
#PlotInputMLEvents(df,"mass")
#PlotInputMLEvents(df,"eCM")
#PlotInputEvents(df)

