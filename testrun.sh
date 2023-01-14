#./ToyMCFlowAI tree_toymcflowAI_bg0NUE0.root 1000 1245 0. 0
seq 3000 3009 | xargs -n1 -P8 -I{} ./ToyMCFlowAI outputs/tree_toymcflowAI_bg0NUE0_{}.root 5000 {} 0. 0