#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

// tree node structure
struct Node {
	char data;
	unsigned freq;
	Node *left, *right;

	Node(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

// comparison object to be used to order the heap
struct compare {
	bool operator()(Node* l, Node* r) {
		return (l->freq > r->freq);
	}
};

void calculateFrequency(const std::string& str, std::unordered_map<char, int>& freq) {
	for (char ch : str) {
		freq[ch]++;
	}
}

void buildHuffmanTree(const std::unordered_map<char, int>& freq, Node* &root) {
	std::priority_queue<Node*, std::vector<Node*>, compare> minHeap;

	for (auto pair : freq) {
		minHeap.push(new Node(pair.first, pair.second));
	}

	while (minHeap.size() != 1) {
		Node *left = minHeap.top();
		minHeap.pop();

		Node *right = minHeap.top();
		minHeap.pop();

		Node *top = new Node('$', left->freq + right->freq);
		top->left = left;
		top->right = right;

		minHeap.push(top);
	}

	root = minHeap.top();
}

void buildCodeTable(Node* root, std::unordered_map<char, std::string> &huffmanCode, std::string str = "") {
	if (!root) {
		return;
	}

	if (!root->left && !root->right) {
		huffmanCode[root->data] = str;
	}

	buildCodeTable(root->left, huffmanCode, str + "0");
	buildCodeTable(root->right, huffmanCode, str + "1");
}

std::string encode(const std::string& str, const std::unordered_map<char, std::string>& huffmanCode) {
	std::string encodedString = "";

	for (char ch : str) {
		encodedString += huffmanCode.at(ch);
	}

	return encodedString;
}

std::string decode(const std::string& encodedStr, Node* root) {
	std::string decodedString = "";
	Node* current = root;

	for (int i = 0; i < encodedStr.size(); i++) {
		if (encodedStr[i] == '0') {
			current = current->left;
		}
		else {
			current = current->right;
		}

		if (!current->left && !current->right) {
			decodedString += current->data;
			current = root;
		}
	}

	return decodedString;
}

int main() {
	std::string text = "Huffman coding is a data compression algorithm.";
	std::unordered_map<char, int> freq;
	calculateFrequency(text, freq);

	Node * root = nullptr;
	buildHuffmanTree(freq, root);

	std::unordered_map<char, std::string> huffmanCode;
	buildCodeTable(root, huffmanCode);

	std::string encodedString = encode(text, huffmanCode);
	std::string decodedString = decode(encodedString, root);

	// output
	std::cout << "Encoded string is:\n" << encodedString << std::endl;
	std::cout << "Decoded string is:\n" << decodedString << std::endl;

	return 0;
}