#pragma once

#include "../Utils/DynamicArray.h"
#include "../Utils/ActivationFunctions.h"
#include "../Utils/Constants.h"

#include <fstream>

#include "Neuron.h"

class NeuralNetwork
{
    private:
        int numOfLayers;
        Utils::DynamicArray<Utils::DynamicArray<Neuron*>*> layers;

        // Weight is order as follows: weights[layer][src_neuron][dst_neuron]
        Utils::DynamicArray<Utils::DynamicArray<Utils::DynamicArray<double>*>*> weights;
        double (*activationFunction)(const double& x);
        double (*activationFunctionDerivative)(const double& x);

        double learningRate;
    public:

        // Destructor and Constructors
        ~NeuralNetwork();
        NeuralNetwork() = delete;
        NeuralNetwork(const Utils::DynamicArray<int>& numOfNeuronsPerLayers, double (*newActivationFunction)(const double& x) = ActivationFunctions::ReLU, double (*newActivationFunctionDerivative)(const double& x) = ActivationFunctions::ReLUDerivative, const double& newLeariningRate = 0.1);
        NeuralNetwork(const char* path, double (*newActivationFunction)(const double& x) = ActivationFunctions::ReLU, double (*newActivationFunctionDerivative)(const double& x) = ActivationFunctions::ReLUDerivative);
        
        // Getters
        Utils::DynamicArray<Utils::DynamicArray<Neuron*>*> GetLayers() const;
        Utils::DynamicArray<Utils::DynamicArray<Utils::DynamicArray<double>*>*> GetWeights() const;
        Utils::DynamicArray<Neuron*>* GetInputLayer() const;
        Utils::DynamicArray<Neuron*>* GetOutputLayer() const;
        double GetWeight(const int& layer_num, const int& src_neuron_number, const int& dst_neuron_number);
        double GetNeuronBias(const int& layer_num, const int& neuron_number);
        double GetLearningRate();
        
        // Setters
        void SetWeight(const int& layer_num, const int& src_neuron_number, const int& dst_neuron_number, const double newValue);
        void SetInputLayer(const Utils::DynamicArray<double>& inputs);
        void SetLearningRate(const double& newLeariningRate);

        // General Functionallity
        void Print();
        void SaveToFile(const char* path);
        double ActivationFunction(const double& x);
        double ActivationFunctionDerivative(const double& x);
        // TODO: Add a save network to file function

        // Network Functionallity
        void PropagateForward();
        void BackPropagate(const Utils::DynamicArray<Utils::DynamicArray<double>>& inputs, const Utils::DynamicArray<Utils::DynamicArray<double>>& expectedOutputs, uint reps = 1);
        int GetMostActiveNeuronIndex();
};

#include "NeuralNetwork.cpp"

