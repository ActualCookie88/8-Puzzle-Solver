# Data Set Feature Selection  

By Luke Matsunaga  

Project completed as part of an Intro to Artificial Intelligence course.  
Focus: Implementing feature selection algorithms for improving classification performance.

This project implements feature selection techniques in C++ to evaluate and identify the most relevant features in a dataset.

## Algorithms Implemented
- Forward Selection  
- Backward Elimination  
- Nearest Neighbor evaluation (leave-one-out validation)

## Prerequisites
- C++17 compiler (GCC, Clang, or MSVC)

## Features
- Supports custom datasets (text file input)  
- Evaluates feature subsets using accuracy metrics  
- Outputs best feature subset and corresponding accuracy  
- Tracks performance across search steps  
- Modular and readable C++ implementation  

## Concepts Covered
- Feature selection in machine learning  
- Search strategies in feature space  
- Model evaluation using classification accuracy  
- Trade-offs between feature subset size and performance

## Dataset: UCI Wine
- Ran Forward Selection and Backward Elimination a real public dataset
- Results can be found in the project report, link below
- Link to dataset: https://archive.ics.uci.edu/dataset/109/wine

## Building the Project
1. Clone the repo (if you haven't already)
```bash
git clone https://github.com/ActualCookie88/IntroToAIProjects.git
```
2. Navigate to folder and build executable
```bash
cd Dataset_Feature_Selection/
g++ -std=c++17 feature_selection.cpp -o feature_selection
```

## Running the Program
```bash
./feature_selection        # Linux/macOS
feature_selection.exe      # Windows
```

You will be prompted to:

Choose the algorithm (Forward Selection or Backward Elimination)
Provide a dataset file

## Output

The program displays:
- Accuracy of each feature subset
- Best feature subset found
- Final classification accuracy
- Program runtime

## Report
For detailed results and analysis, see project report:
[View Project Report](report/Feature-SelectionProjectReport.pdf)
