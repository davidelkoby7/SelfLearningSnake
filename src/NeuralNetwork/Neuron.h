#pragma once

#include "../Utils/DynamicArray.h"
#include "../Utils/GeneralFunctions.cpp"

class Neuron
{
    private:
        double m_value;
        double m_value_no_activation;
        double m_bias;
        double m_delta; // Holds the error of that neuron for the learning process
        Utils::DynamicArray<Neuron*> m_nextLayerNeurons;
    public:
        // Constructos and Destructor
        ~Neuron();
        Neuron();
        Neuron(const double& newValue, const double& newBias);
        Neuron(const double& newValue, const double& newBias, const Utils::DynamicArray<Neuron*>& nextLayerNeurons);
        
        // Setters
        void SetValue(const double& newValue);
        void SetValueNoActivation(const double& newValue);
        void SetBias(const double& newBias);
        void SetDelta(const double& newDelta);
        
        // Getters
        double GetValue() const;
        double GetValueNoActivation() const;
        double GetBias() const;
        double GetDelta() const;
        Utils::DynamicArray<Neuron*>& GetNextLayerNeurons();

        // Additional Functionallities
        std::string ToString() const;
        std::string ToStringNoNextLayer() const;
        void Print();
        void PrintNoNextLayer();
};

#include "Neuron.cpp"

