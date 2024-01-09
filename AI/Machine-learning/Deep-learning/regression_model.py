"""
Author: Bryce Calhoun
Description: Implementation of a simple linear regression model made to learn
             the pattern: (y = x). Using tensorflow to produce the data sets,
             keras for the library of neural networks, and matlab plot
             library to display the data.

"""
import tensorflow as tf
import matplotlib.pyplot as plt
import keras
import numpy as np


# setting random generator seeds to get consistent results.
tf.random.set_seed(42)
np.random.seed(42)

"""
    Making a training set of data as well as a testing set of data.
    After fitting the model with training data. The model's learning 
    ability will be tested and evaluated with the test data. 
    Pattern: y=x.
"""
inputs_test = tf.constant([7.0, 8.0, 9.0])
outputs_test = tf.constant([7.0, 8.0, 9.0])

inputs_training = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0])
outputs_training = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0])


"""
    Using MATLAB's plotting library to scatter plot the data points,
    blue dots show training data and red dots show the testing data.
"""
plt.scatter(inputs_training, outputs_training, c="b")
plt.scatter(inputs_test, outputs_test, c="r")
plt.show()


"""
    Defining a linear regression model with 1 neuron in the input layer, 1 neuron in the output layer
    and a single, fully-connected hidden layer with 10 neurons and a linear activation function.
    1 input feature, 1 output feature.
"""
regression = keras.Sequential([
    keras.layers.Input(shape=(1,)),
    keras.layers.Dense(10, activation=None),
    keras.layers.Dense(1, activation=None)
])


# tried to use non-stochastic gradient descent to reduce overall
# randomness and stabilize results here,
# results were significantly less stable and more random.

#plain_gradient_descent = keras.optimizers.SGD(learning_rate=1.0)


""" 
     Compiling the model with the "mean absolute average" loss
     function and a stochastic gradient descent optimizer. No metrics
     function is used.
"""
regression.compile(
    loss="mae",
    optimizer=keras.optimizers.SGD()
)


"""
    Fitting the model with the training data for 10 epochs without printing 
    the results.
"""
regression.fit(inputs_training, outputs_training, epochs=10, verbose=1)


"""
    Comparing the expected results of a new prediction with the actual results from
    the model.
"""
#print("\nExpected: ", outputs_test, "\n", regression.predict(inputs_test))
#print("\n--------------------------------\n")
