#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Question {
    string text;
    vector<string> options;
};

void askQuestion(const Question& q, vector<int>& answers) {
    cout << q.text << endl;
    for (int i = 0; i < q.options.size(); ++i) {
        cout << i + 1 << ". " << q.options[i] << endl;
    }
    int answer;
    cin >> answer;
    answers.push_back(answer);
}

void loadTrainingData(const string& filename, vector<vector<int>>& samples, vector<int>& labels) {
    ifstream dataFile(filename);
    if (!dataFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(dataFile, line)) {
        istringstream iss(line);
        vector<int> sample;
        int value;
        while (iss >> value) {
            sample.push_back(value);
        }
        labels.push_back(sample.back());
        sample.pop_back();
        samples.push_back(sample);
    }
}

int predictParty(const vector<int>& answers, const vector<vector<int>>& samples, const vector<int>& labels) {
    vector<int> partyCount(4, 0); // Assuming 4 political parties

    for (size_t i = 0; i < samples.size(); ++i) {
        int matchCount = 0;
        for (size_t j = 0; j < answers.size(); ++j) {
            if (answers[j] == samples[i][j]) {
                matchCount++;
            }
        }
        partyCount[labels[i]] += matchCount;
    }

    int predictedParty = distance(partyCount.begin(), max_element(partyCount.begin(), partyCount.end()));
    return predictedParty;
}

int main() {
    vector<Question> survey = {
        {"What should the government do to help the poor?", {"Make it easier to apply for assistance.", "Allow parents to use education funds for charter schools.", "Create welfare-to-work programs.", "Nothing."}},
        {"How should the government handle healthcare?", {"Implement a universal healthcare system.", "Provide tax credits for private health insurance.", "Expand Medicaid and Medicare.", "Leave it to the private sector."}},
        {"What is your stance on climate change?", {"The government should take immediate action to reduce carbon emissions.", "Encourage private companies to innovate and reduce emissions.", "Implement a mix of government regulations and incentives.", "Climate change is not a significant issue."}},
        {"How should the government approach education?", {"Increase funding for public schools.", "Support school choice and charter schools.", "Provide more vocational training programs.", "Reduce government involvement in education."}},
        {"What is your opinion on immigration?", {"Provide a pathway to citizenship for undocumented immigrants.", "Strengthen border security and enforce existing laws.", "Implement a merit-based immigration system.", "Reduce immigration levels overall."}},
        {"How should the government handle taxes?", {"Increase taxes on the wealthy to fund social programs.", "Lower taxes to stimulate economic growth.", "Simplify the tax code and close loopholes.", "Maintain current tax levels."}},
        {"What is your stance on gun control?", {"Implement stricter gun control laws.", "Protect the Second Amendment rights.", "Enforce existing gun laws more effectively.", "Reduce regulations on gun ownership."}},
        {"How should the government address economic inequality?", {"Increase the minimum wage and provide more social services.", "Promote free-market policies to create jobs.", "Implement progressive taxation and social programs.", "Economic inequality is not a significant issue."}},
        {"What is your opinion on foreign policy?", {"Focus on diplomacy and international cooperation.", "Strengthen the military and take a tough stance on adversaries.", "Balance military strength with diplomatic efforts.", "Reduce involvement in international affairs."}},
        {"Which political party do you affiliate with?", {"Democratic Party", "Republican Party", "Libertarian Party", "Green Party"}}
    };

    vector<int> answers;
    for (const auto& q : survey) {
        askQuestion(q, answers);
    }

    // Load the training data
    vector<vector<int>> samples;
    vector<int> labels;
    loadTrainingData("survey_data.txt", samples, labels);

    // Predict the user's political party
    int predictedParty = predictParty(answers, samples, labels);

    cout << "Based on your answers, we predict your political party is: ";
    switch (predictedParty) {
        case 0: cout << "Democratic Party" << endl; break;
        case 1: cout << "Republican Party" << endl; break;
        case 2: cout << "Libertarian Party" << endl; break;
        case 3: cout << "Green Party" << endl; break;
    }

    // Append the user's answers to the data file
    ofstream dataFileAppend("survey_data.txt", ios::app);
    for (const auto& answer : answers) {
        dataFileAppend << answer << " ";
    }
    dataFileAppend << endl;

    cout << "Thank you for completing the survey!" << endl;

    return 0;
}
