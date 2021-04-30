import numpy as np
import ast
import os

def initFromFile(fileDir): #loads previously saved training data
    try:
        save = open(fileDir,"r")
        saveData = ast.literal_eval(save.read()) #returns layers, weights, biases
        net = Perceptron(saveData[0],saveData[1],saveData[2])
        return net
    except FileNotFoundError: #in case a file is inputted that does not exist
        print("invalid save")
        return None

class Perceptron:
    def __init__(self,layers,weights=None,biases=None):
        self.layerList = layers #the structure of the neural net
        self.weightsList = [0] #all the weights
        self.biasesList = [0] #all the biases
        self.valuesList = [0]*len(self.layerList) #records the activations during foward propagation
        self.derivs = [0]*len(self.layerList) #records the derivatives of the activations
        self.wgrads = [0]*len(self.layerList) #the gradients of all the weights
        self.bgrads = [0]*len(self.layerList) #the gradients of all the biases
        if weights == None or biases == None:
            for i in range(1,len(self.layerList)): #initializes weights and biases
                self.weightsList.append((np.random.rand(self.layerList[i],self.layerList[i-1])-1)/(np.sqrt(self.layerList[i-1])))
                self.biasesList.append((np.random.rand(self.layerList[i],1)-1)/(np.sqrt(self.layerList[i-1])))
        else:
            for i in range(1,len(self.layerList)): #allows for the use of previously trained nets
                self.weightsList.append(np.array(weights[i]).reshape(self.layerList[i],self.layerList[i-1]))
                self.biasesList.append(np.array(biases[i]).reshape(len(biases[i]),1))

    def computeOutput(self,inputList): #foward propagation/returns output activations
        self.valuesList = [0]*len(self.layerList) #resets all the activation
        self.derivs = [0]*len(self.layerList) #resets the derivatives of the activations
        self.valuesList[0] = np.array(inputList,dtype=np.float32).reshape(len(inputList),1) #sets 
        for i in range(1,len(self.layerList)): #traverses each layer with an activation
            self.valuesList[i] = 1.0/(1+np.exp(-(self.weightsList[i] @ self.valuesList[i-1] + self.biasesList[i]))) #saves the activation from that layer
            self.derivs[i] = self.valuesList[i] * (1-self.valuesList[i]) #saves the derivative of that activation
        return self.valuesList[len(self.layerList)-1].tolist() #returns output (activation of the final layer)

    def backprop(self,actualList,LR): #calculates gradient for a single input and adds it to previously calculated gradients
        delta = self.derivs[len(self.layerList)-1]*(self.valuesList[len(self.layerList)-1] - np.array(actualList).reshape(len(actualList),1)) #used to calculate gradient
        for i in range(len(self.layerList)-1,0,-1): #traverses each layer with weight(s)/bias(es) backwards
            self.wgrads[i] += LR * (delta @ self.valuesList[i-1].transpose()) #weight gradients of layer i
            self.bgrads[i] += LR * delta #bias gradients of layer i
            delta = self.derivs[i-1] * (self.weightsList[i].transpose() @ delta) #updates for the next layer
    
    def applyGradient(self): #applies and resets previously calculated gradients
        for i in range(1,len(self.layerList)): #traverses each layer with weight(s)/bias(es)
            self.weightsList[i] -= self.wgrads[i] #applies weight gradient
            self.biasesList[i] -= self.bgrads[i] #applies bias gradient
        self.wgrads = [0]*len(self.layerList) #resets weight gradients
        self.bgrads = [0]*len(self.layerList) #resets bias gradients
        
    def train(self,inputList,actualList,LR=1):
        self.computeOutput(inputList)
        self.backprop(actualList,LR)
        self.applyGradient()

    def writeSave(self,fileName): #saves training data in a way that if you stop mid-write it will not delete your training data
        if os.path.exists("tempSave.txt"): #would not recomend keeping a file called tempSave.txt
            os.remove("tempSave.txt")
        save = open("tempSave.txt","w+") 
        weights = [0]
        biases = [0]
        for i in range(1,len(self.layerList)):
            weights.append(self.weightsList[i].tolist()) #transforms weightList to a list of lists
            biases.append(self.biasesList[i].tolist()) #transforms biasList to a list of list
        saveData = [] #combines them for easy read/write
        saveData.append(self.layerList)
        saveData.append(weights)
        saveData.append(biases)
        save.write(str(saveData)) #writes save data to tempSave.txt
        if os.path.exists(fileName): #deletes old save if it exists
            os.remove(fileName)
        save.close()
        save = open(fileName,"w+")
        save.write(str(saveData)) #writes save data to the specified save file
        os.remove("tempSave.txt") #deletes the temporary save file
        save.close()

if __name__ == '__main__':
    net = Perceptron([5,4,2])
    net.writeSave('Voter Net.sav')