#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

// Basic stopwords list
set<string> stopwords = {"the", "is", "in", "at", "of", "a", "and", "to", "it", "on", "for", "with", "as", "by", "an"};

// Split text into sentences
vector<string> splitSentences(const string& text) {
    vector<string> sentences;
    stringstream ss(text);
    string sentence;
    while (getline(ss, sentence, '.')) {
        if (!sentence.empty()) {
            sentences.push_back(sentence + ".");
        }
    }
    return sentences;
}

// Tokenize sentence into words
vector<string> tokenize(const string& sentence) {
    vector<string> words;
    stringstream ss(sentence);
    string word;
    while (ss >> word) {
        // Remove punctuation
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
        // Convert to lowercase
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!stopwords.count(word)) {
            words.push_back(word);
        }
    }
    return words;
}

// Score each sentence based on word frequency
map<string, double> scoreSentences(const vector<string>& sentences) {
    map<string, double> scores;
    map<string, int> wordFreq;

    // Count word frequencies
    for (const auto& sentence : sentences) {
        for (const auto& word : tokenize(sentence)) {
            wordFreq[word]++;
        }
    }

    // Score sentences
    for (const auto& sentence : sentences) {
        double score = 0;
        for (const auto& word : tokenize(sentence)) {
            score += wordFreq[word];
        }
        scores[sentence] = score;
    }

    return scores;
}

// Get top N sentences
vector<string> summarize(const string& text, int numSentences = 2) {
    auto sentences = splitSentences(text);
    auto scores = scoreSentences(sentences);

    // Sort sentences by score
    vector<pair<string, double>> sorted(scores.begin(), scores.end());
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    vector<string> summary;
    for (int i = 0; i < min(numSentences, (int)sorted.size()); ++i) {
        summary.push_back(sorted[i].first);
    }
    return summary;
}

int main() {
    string text = "Artificial intelligence is transforming the world. It is used in healthcare, finance, and transportation. "
                  "Machine learning is a subset of AI. It allows systems to learn from data. "
                  "Natural language processing helps computers understand human language. "
                  "AI is also used in robotics and automation.";

    auto summary = summarize(text, 2);
    cout << "\n📝 Summary:\n";
    for (const auto& sentence : summary) {
        cout << "- " << sentence << endl;
    }

    return 0;
}

