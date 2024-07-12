"""
A sequence of functions designed to take the accurate labels of a binary classification problem
of 1s and 0s, in conjunction with the predicted labels and return  the sensitivity and specificity
"""


def get_true_pos_neg(accurate_labels):
    True_Positives = 0
    True_Negatives = 0

    for x in range(len(accurate_labels)):
        if labels[x] == 1:
            True_Positives += 1
        else:
            True_Negatives += 1

    return True_Positives, True_Negatives


def get_false_pos_neg(accurate_labels, predicted_labels):

    False_Positives = 0
    False_Negatives = 0

    for x in range(len(predictions)):
        if predicted_labels[x] != accurate_labels[x]:
            if predicted_labels[x] == 1:
                False_Positives += 1
            else:
                False_Negatives += 1

    return False_Positives, False_Negatives

def calculate_sensitivity(True_Positives, False_Negatives):

    Sensitivity = float(True_Positives / (True_Positives + False_Negatives))

    return Sensitivity



def calculate_specificity(True_Negatives, False_Positives):

    Specificity = float(True_Negatives / (True_Negatives + False_Positives))

    return Specificity

def evaluate(accurate_labels, predicted_labels):
    True_Positives, True_Negatives = get_true_pos_neg(accurate_labels)
    False_Positives, False_Negatives = get_false_pos_neg(accurate_labels, predicted_labels)

    Sensitivity = calculate_sensitivity(True_Positives, False_Negatives)
    Specificity = calculate_specificity(True_Negatives, False_Positives)

    return Sensitivity, Specificity
