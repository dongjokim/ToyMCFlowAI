#./ToyMCFlowAI tree_toymcflowAI_bg0NUE0.root 1000 1245 0. 0
seq 1000 1007 | xargs -n1 -P8 -I{} ./ToyMCFlowAI tree_toymcflowAI_bg0NUE0_{}.root 1000 {} 0. 0
