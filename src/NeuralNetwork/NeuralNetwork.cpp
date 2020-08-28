#include "NeuralNetwork.h"

NeuralNetwork::~NeuralNetwork()
{
}

NeuralNetwork::NeuralNetwork(const Utils::DynamicArray<int>& numOfNeuronsPerLayer, double (*newActivationFunction)(const double& x), double (*newActivationFunctionDerivative)(const double& x), const double& newLeariningRate)
{
    // Setting the activation function
    this->activationFunction = newActivationFunction;
    this->activationFunctionDerivative = newActivationFunctionDerivative;

    this->learningRate = newLeariningRate;
    this->numOfLayers = numOfNeuronsPerLayer.GetLength();

    // Initializing a random valued layers for the network
    for (int i = 0; i < this->numOfLayers; i++)
    {
        // Creating the new layer
        Utils::DynamicArray<Neuron*>* newLayer = new Utils::DynamicArray<Neuron*>();

        // Filling the layer with random nodes
        for (int j = 0; j < numOfNeuronsPerLayer[i]; j++)
        {
            Neuron *n = new Neuron();
            newLayer->AddItem(n);
        }

        this->layers.AddItem(newLayer);
    }

    // Initializing the weights array with randomized values
    for (int i = 0; i < this->numOfLayers - 1; i++)
    {
        Utils::DynamicArray<Utils::DynamicArray<double>*>* currLayerWeights = new Utils::DynamicArray<Utils::DynamicArray<double>*>();

        // Running over each of the neurons of the src layer
        for (int j = 0; j < numOfNeuronsPerLayer[i]; j++)
        {
            Utils::DynamicArray<double>* currNeuronWeights = new Utils::DynamicArray<double>();

            // For each of the neurons in the current layer - add a random weight to the next layer's neuron's DynamicArray.
            for (int k = 0; k < numOfNeuronsPerLayer[i + 1]; k++)
            {
                double randWeight = GeneralFunctions::RandomDouble(-1, 1);
                currNeuronWeights->AddItem(randWeight);
            }
            currLayerWeights->AddItem(currNeuronWeights);
        }
        this->weights.AddItem(currLayerWeights);
    }
}

NeuralNetwork::NeuralNetwork(const char* path, double (*newActivationFunction)(const double& x), double (*newActivationFunctionDerivative)(const double& x))
{
    // Setting the activation function
    this->activationFunction = newActivationFunction;
    this->activationFunctionDerivative = newActivationFunctionDerivative;
    
    // Reading the file content
    std::string nnFileContent = GeneralFunctions::ReadFile(path);
    Utils::DynamicArray<std::string> lines = GeneralFunctions::SplitString(nnFileContent, "\n");

    // Reading the learning rate and the num of layers
    this->learningRate = atof(GeneralFunctions::SplitString(lines[Constants::LEARNING_RATE_LINE], " = ")[1].c_str());
    this->numOfLayers = atoi(GeneralFunctions::SplitString(lines[Constants::NUM_OF_LAYERS_LINE], " = ")[1].c_str());

    // Reading the number of neurons per layer
    std::string numOfNeuronsPerLayerString = GeneralFunctions::SplitString(lines[Constants::NUM_OF_NEURONS_PER_LAYER_LINE], " = ")[1];
    Utils::DynamicArray<std::string> numOfNeuronsPerLayerSplittedString = GeneralFunctions::SplitString(numOfNeuronsPerLayerString, ",");
    Utils::DynamicArray<int> numOfNeuronsPerLayer;
    for (uint i = 0; i < numOfNeuronsPerLayerSplittedString.GetLength(); i++)
        numOfNeuronsPerLayer.AddItem(atoi(numOfNeuronsPerLayerSplittedString[i].c_str()));

    int currentLine = Constants::NEURONS_BIASES_STARTING_LINE;

    // Creating the neurons with their biases
    for (uint i = 0; i < this->numOfLayers; i++) // Running over each layer
    {
        // Creating the new layer
        Utils::DynamicArray<Neuron*>* newLayer = new Utils::DynamicArray<Neuron*>();

        // Reading the layer's biases
        std::string currentLayerBiasesLine = GeneralFunctions::SplitString(lines[currentLine], " = ")[1];
        Utils::DynamicArray<std::string> currentLayerBiasesStringArr = GeneralFunctions::SplitString(currentLayerBiasesLine, ",");
        Utils::DynamicArray<double> currentLayerBiases;
        for (uint j = 0; j < currentLayerBiasesStringArr.GetLength(); j++)
            currentLayerBiases.AddItem(atof(currentLayerBiasesStringArr[j].c_str()));

        // Running over each neuron in the layer
        for (uint j = 0; j < numOfNeuronsPerLayer[i]; j++)
        {
            Neuron *n = new Neuron(0, currentLayerBiases[j]);
            newLayer->AddItem(n);
        }

        this->layers.AddItem(newLayer);
        currentLine++;
    }

    // currentLine is now pointing at the first line representing weights

    // Creating the weights of the network
    for (uint i = 0; i < this->numOfLayers - 1; i++)
    {
        Utils::DynamicArray<Utils::DynamicArray<double>*>* currLayerWeights = new Utils::DynamicArray<Utils::DynamicArray<double>*>();

        // Running over each of the neurons in the src layer
        for (uint j = 0; j < numOfNeuronsPerLayer[i]; j++)
        {
            // Reading the new weights from the file
            std::string currentNeuronWeightsLine = GeneralFunctions::SplitString(lines[currentLine], " = ")[1];
            Utils::DynamicArray<std::string> currentNeuronWeightsStringArr = GeneralFunctions::SplitString(currentNeuronWeightsLine, ",");

            Utils::DynamicArray<double>* currentNeuronWeights = new Utils::DynamicArray<double>();
            for (uint k = 0; k < currentNeuronWeightsStringArr.GetLength(); k++)
            {
                double newWeight = atof(currentNeuronWeightsStringArr[k].c_str());
                currentNeuronWeights->AddItem(newWeight);
            }

            currLayerWeights->AddItem(currentNeuronWeights);
            currentLine++;
        }
        this->weights.AddItem(currLayerWeights);
    }
}

Utils::DynamicArray<Utils::DynamicArray<Neuron*>*> NeuralNetwork::GetLayers() const
{
    return this->layers;
}

Utils::DynamicArray<Utils::DynamicArray<Utils::DynamicArray<double>*>*> NeuralNetwork::GetWeights() const
{
    return this->weights;
}

Utils::DynamicArray<Neuron*>* NeuralNetwork::GetInputLayer() const
{
    return this->layers[0];
}

Utils::DynamicArray<Neuron*>* NeuralNetwork::GetOutputLayer() const
{
    return this->layers[this->layers.GetLength() - 1];
}

double NeuralNetwork::GetWeight(const int& layer_num, const int& src_neuron_number, const int& dst_neuron_number)
{
    return this->weights[layer_num]->GetItem(src_neuron_number)->GetItem(dst_neuron_number);
}

double NeuralNetwork::GetNeuronBias(const int& layer_num, const int& neuron_number)
{
    return this->layers.GetItem(layer_num)->GetItem(neuron_number)->GetBias();
}

double NeuralNetwork::GetLearningRate()
{
    return this->learningRate;
}

void NeuralNetwork::SetWeight(const int& layer_num, const int& src_neuron_number, const int& dst_neuron_number, const double newValue)
{
    double& currentWeight = this->weights[layer_num]->GetItem(src_neuron_number)->GetItem(dst_neuron_number);
    currentWeight = newValue;
}

void NeuralNetwork::SetInputLayer(const Utils::DynamicArray<double>& inputs)
{
    if (this->layers[0]->GetLength() != inputs.GetLength())
    {
        throw "Input given not in the correct length!";
    }

    for (uint i = 0; i < this->layers[0]->GetLength(); i++)
    {
        this->layers[0]->GetItem(i)->SetValue(inputs[i]);
    }
}

void NeuralNetwork::SetLearningRate(const double& newLearningRate)
{
    this->learningRate = newLearningRate;
}

void NeuralNetwork::Print()
{
    std::cout << "|| Printing Neural Network ||\n";
    std::cout << "\n-- Learning Rate: " << this->learningRate << " --\n\n";
    // Running over the layers
    for (uint i = 0; i < this->layers.GetLength(); i++)
    {
        std::cout << "~~ Printing layer number: " << i << " ~~\n";

        // Running over each Neuron in that layer
        for (uint j = 0; j < this->layers[i]->GetLength(); j++)
        {
            std::cout << "@@ Printing neuron number: " << j << " @@\n";
            this->layers[i]->GetItem(j)->PrintNoNextLayer();
        }
    }

    std::cout << "~~ Printing Weights ~~\n";
    // Running over all the weights (Layers Loop)
    for (uint i = 0; i < this->weights.GetLength(); i++)
    {
        Utils::DynamicArray<Utils::DynamicArray<double>*>* currentLayerWeights = this->weights.GetItem(i);

        // Running over the current layer neuron array
        for (uint j = 0; j < currentLayerWeights->GetLength(); j++)
        {
            Utils::DynamicArray<double>* currentNeuronWeights = currentLayerWeights->GetItem(j);

            // Running over all the weights of that neuron
            for (uint k = 0; k < currentNeuronWeights->GetLength(); k++)
            {
                std::cout << "[ layer: " << i << ", srcNeuron: " << j << ", dstNeuron: " << k << " ]:" << currentNeuronWeights->GetItem(k) << "\n";
            }
        }
    }
}

void NeuralNetwork::SaveToFile(const char* path)
{
    // General: Not using the write/append functions in the GeneralFunctions for reasons of efficiency (creating / deleting the file stream every time we add somethin).

    // Removing the file if it exists
    GeneralFunctions::RemoveFile(path);

    // Opening the file
    std::fstream nnFile;
    nnFile.open(path, std::ios::app);

    // Adding the learning rate, num of layers and num of neurons per layer
    nnFile << "learning_rate = " << this->learningRate << "\n";
    nnFile << "num_of_layers = " << this->numOfLayers << "\n";
    nnFile << "num_of_neurons_per_layers = ";

    for (uint i = 0; i < this->numOfLayers; i++)
    {
        nnFile << this->layers[i]->GetLength();
        if (i != this->layers.GetLength() - 1)
            nnFile << ",";
    }
    nnFile << "\n";

    // Adding the biases of each neuron (with a newline between each layer)
    for (uint i = 0; i < this->numOfLayers; i++)
    {
        nnFile << "biases_layer_" << i << " = ";
        const int currLayerLength = this->layers[i]->GetLength();
        for (uint j = 0; j < currLayerLength; j++)
        {
            nnFile << this->layers[i]->GetItem(j)->GetBias();
            if (j != currLayerLength - 1)
                nnFile << ",";
        }
        nnFile << "\n";
    }

    // Adding the weights of the network
    for (uint i = 0; i < this->weights.GetLength(); i++)
    {
        for (uint j = 0; j < this->weights[i]->GetLength(); j++)
        {
            nnFile << "weights_layer_" << i << "_neuron_" << j << " = ";
            for (uint k = 0; k < this->layers[i + 1]->GetLength(); k++)
            {
                nnFile << this->GetWeight(i, j, k);
                if (k != this->layers[i + 1]->GetLength() - 1)
                    nnFile << ",";
            }
            if (! (j == this->weights[i]->GetLength() - 1 && i == this->weights.GetLength() - 1))
                nnFile << "\n";
        }
    }
}

void NeuralNetwork::PropagateForward()
{
    // Running over all the layers from the secend layer (the first layer is the input layer)
    for (uint i = 1; i < this->numOfLayers; i++)
    {
        Utils::DynamicArray<Neuron*>* currentLayer = this->layers.GetItem(i);
        const int currentLayerLength = currentLayer->GetLength();
        Utils::DynamicArray<Neuron*>* previousLayer = this->layers.GetItem(i - 1);
        const int previousLayerLength = previousLayer->GetLength();

        // Running over each neuron in the layer to update it's value
        for (uint j = 0; j < currentLayerLength; j++)
        {
            Neuron* currentNeuron = currentLayer->GetItem(j);
            double newCurrentNeuronValue = 0;

            // Running over each neuron in the previous layer to propagate it forward
            for (uint k = 0; k < previousLayerLength; k++)
                newCurrentNeuronValue += previousLayer->GetItem(k)->GetValue() * this->GetWeight(i - 1, k, j);

            newCurrentNeuronValue += currentNeuron->GetBias();
            currentNeuron->SetValueNoActivation(newCurrentNeuronValue); // Setting the value without the activation

            newCurrentNeuronValue = this->activationFunction(newCurrentNeuronValue);
            currentNeuron->SetValue(newCurrentNeuronValue); // Setting the value with the activation
        }
    }
}

void NeuralNetwork::BackPropagate(const Utils::DynamicArray<Utils::DynamicArray<double>>& inputs, const Utils::DynamicArray<Utils::DynamicArray<double>>& expectedOutputs, uint reps)
{
    // Making sure the output is in the correct length
    if (expectedOutputs.GetLength() != inputs.GetLength())
        throw "Mismatch in the length of the inputs and the outputs";

    for (size_t rep = 0; rep < reps; rep++)
    {
        // Running over all the training data
        const int numOfInputs = inputs.GetLength();
        for (uint currentInput = 0; currentInput < numOfInputs; currentInput++)
        {
            // Printing the progress every 100 samples trained
            if (currentInput % 100 == 0)
                std::cout << "Repetition: " << (rep + 1) << "/" << reps << ", currentInput: " << currentInput << "\n";

            // Setting the input layer to the given data
            this->SetInputLayer(inputs[currentInput]);
            this->PropagateForward();

            // Running over all the neuron layers from the last layer to the first
            for (int i = this->numOfLayers - 1; i >= 0; i--)
            {
                // Running over each neuron in the current layer (i)
                const int currLayerLength = this->layers[i]->GetLength();
                for (uint j = 0; j < currLayerLength; j++)
                {
                    Neuron* currentNeuron = this->layers[i]->GetItem(j);

                    // If the output layer neuron - check it's error with the expectedOutputs array
                    if (i == this->numOfLayers - 1)
                    {
                        // Updating the neuron's delta value (error value)
                        double newDelta = currentNeuron->GetValue() - expectedOutputs[currentInput].GetItem(j);
                        double activationDerivativeResult = this->activationFunctionDerivative(currentNeuron->GetValueNoActivation());
                        newDelta *= activationDerivativeResult;
                        currentNeuron->SetDelta(newDelta);

                        // Improving the bias of that neuron
                        currentNeuron->SetBias(currentNeuron->GetBias() - this->learningRate * newDelta);
                    }
                    else // If the neuron is not in the output layer
                    {
                        const int nextLayerLength = this->layers[i + 1]->GetLength();

                        // Calculating the new delta (error) of the neuron
                        double newDelta = 0;

                        // Adding the influence of this neuron over the next layer neurons
                        for (uint k = 0; k < nextLayerLength; k++)
                        {
                            double deltaAddition;
                            deltaAddition = this->layers[i + 1]->GetItem(k)->GetDelta();
                            deltaAddition *= this->GetWeight(i, j, k);
                            newDelta += deltaAddition;
                        }

                        newDelta *= this->ActivationFunctionDerivative(currentNeuron->GetValueNoActivation());
                        currentNeuron->SetDelta(newDelta);

                        // Updating the bias of that neuron
                        currentNeuron->SetBias(currentNeuron->GetBias() - this->learningRate * newDelta);

                        // Running over all the weights connected to that neuron and improving them
                        for (uint k = 0; k < nextLayerLength; k++)
                        {
                            double& currentWeight = this->weights[i]->GetItem(j)->GetItem(k);
                            currentWeight -= currentNeuron->GetValue() * this->layers[i + 1]->GetItem(k)->GetDelta() * this->learningRate;
                        }
                    }
                }
            }
        }
    }
}

double NeuralNetwork::ActivationFunction(const double& x)
{
    return this->activationFunction(x);
}

double NeuralNetwork::ActivationFunctionDerivative(const double& x)
{
    return this->activationFunctionDerivative(x);
}

int NeuralNetwork::GetMostActiveNeuronIndex()
{
    Utils::DynamicArray<Neuron*>* lastLayer = this->layers[numOfLayers - 1];
    uint lastLayerLength = lastLayer->GetLength();
    int mostActiveIndex = 0;
    double highestValue = lastLayer->GetItem(0)->GetValue();

    for (uint i = 1; i < lastLayerLength; i++)
    {
        if (lastLayer->GetItem(i)->GetValue() > highestValue)
        {
            highestValue = lastLayer->GetItem(i)->GetValue();
            mostActiveIndex = i;
        }
    }

    return mostActiveIndex;
}

