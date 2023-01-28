#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include<cmath>

#define MAX_TREE_HT 10000

using namespace std;

struct MinHeapNode {
	char data;
	unsigned freq;
	struct MinHeapNode* left, * right;
};

struct MinHeap {
	unsigned size;
	unsigned capacity;
	struct MinHeapNode** array;
};

struct MinHeapNode* newNode(char data, unsigned freq) {
	struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

struct MinHeap* createMinHeap(unsigned capacity) {

	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));

	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx) {

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) {
		smallest = left;
	}

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) {
		smallest = right;
	}

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
			&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

int isSizeOne(struct MinHeap* minHeap) {
	return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap) {

	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap) {

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i) {
		minHeapify(minHeap, i);
	}
}

int isLeaf(struct MinHeapNode* root) {

	return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i) {
		minHeap->array[i] = newNode(data[i], freq[i]);
	}

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
	struct MinHeapNode* left, * right, * top;

	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);


	while (!isSizeOne(minHeap)) {


		left = extractMin(minHeap);
		right = extractMin(minHeap);

		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}


	return extractMin(minHeap);
}

string getCode(int arr[], int n) {
	string total = "";
	string current;
	for (int i = 0; i < n; ++i) {
		stringstream ss;
		ss << arr[i];
		ss >> current;
		total += current;
	}
	return total;
}

void insertCodeInMap(struct MinHeapNode* root, int arr[], int top, map<char, string>& charencode) {

	string code;
	if (root->left) {
		arr[top] = 0;
		insertCodeInMap(root->left, arr, top + 1, charencode);
	}
	if (root->right) {
		arr[top] = 1;
		insertCodeInMap(root->right, arr, top + 1, charencode);
	}

	if (isLeaf(root)) {
		code = getCode(arr, top);
		charencode.insert(pair<char, string>(root->data, code));
	}
}

void HuffmanCodes(char data[], int freq[], int size, map<char, string>& charencode) {

	struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

	int arr[MAX_TREE_HT], top = 0;

	insertCodeInMap(root, arr, top, charencode);
}

int binaryToDecimal(string n)
{
	int num = stoi(n);
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = num;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}
string decimalToBinary(int n) {
	int binaryNum[8];

	// counter for binary array
	for (int i = 0; i < 8; i++) {
		binaryNum[i] = 0;
	}

	int i = 0;
	while (n > 0) {

		// storing remainder in binary array
		binaryNum[i] = n % 2;
		n = n / 2;
		i++;
	}

	// printing binary array in reverse order
	string bit;
	string binaryequivelant = "";
	for (int j = 7; j >= 0; j--) {
		stringstream ss;
		ss << binaryNum[j];
		ss >> bit;
		binaryequivelant += bit;
	}

	return binaryequivelant;
}

int compress(map<char, string> charencode, map<string, int>& mapcount) {
	map<string, int>::iterator itr2;
	map<char, string>::iterator itr;
	string path;
	string file;
	ifstream filein;
	cout << "Please enter the path of the file to be read: ";
	cin >> path;
	filein.open(path);
	while (!filein) {
		cout << "ERROR!\nPlease enter the path of the file to be read: ";
		cin >> path;
		filein.open(path);
	}
	istreambuf_iterator<char> inputIt(filein), emptyInputIt;
	back_insert_iterator<string> stringInsert(file);

	copy(inputIt, emptyInputIt, stringInsert);

	int i = 0;
	while (file.substr(i, 1) != "\0") {
		if (mapcount.find(file.substr(i, 1)) != mapcount.end()) {
			itr2 = mapcount.find(file.substr(i, 1));
			itr2->second++;
		}
		else {
			mapcount.insert(pair<string, int>(file.substr(i, 1), 1));
		}
		i++;
	}

	itr2 = mapcount.begin();
	int totalcount = 0;
	for (int i = 0; i < mapcount.size(); i++) {
		totalcount += itr2->second;
		itr2++;
	}

	char* arr = new char[mapcount.size()];
	int* freq = new int[mapcount.size()];
	itr2 = mapcount.begin();
	for (int i = 0; i < mapcount.size(); i++) {
		//cout << charitr->first << ": " << charitr->second << endl; // remove the comment to see the frequency of each distict character
		arr[i] = itr2->first[0];
		freq[i] = itr2->second;
		itr2++;
	}

	int size = mapcount.size();
	HuffmanCodes(arr, freq, size, charencode);

	itr2 = mapcount.begin();
	double sum = 0;
	double entropy = 0;
	for (int i = 0; i < mapcount.size(); i++) {
		itr = charencode.find(itr2->first[0]);
		int freq = itr2->second;
		int length = (itr->second).length();
		entropy += ((freq + 0.0) / totalcount) * log(1.0 / ((freq + 0.0) / totalcount));
		sum += (length + 0.0) * ((freq + 0.0) / totalcount);
	}
	double compressionratio = sum / 8;
	double efficiency = entropy / sum;
	cout << "The compression ratio is " << compressionratio << " and the efficiency is " << efficiency << endl;
	ofstream mapofcode;
	mapofcode.open("mapofcode.txt");

	itr = charencode.begin();
	char last = '\0';
	string laststr = "";
	for (int i = 0; i < charencode.size(); i++) {
		char chr = itr->first;
		string str = itr->second;
		if (i < (charencode.size() - 1)) {
			if (chr == '\n') {
				mapofcode << str << endl;
				mapofcode << "end" << endl;
				itr++;
			}
			else if (chr == ' ') {
				mapofcode << str << endl;
				mapofcode << "space" << endl;
				itr++;
			}
			else {
				mapofcode << str << endl;
				mapofcode << chr << endl;
				itr++;
			}
		}
		else {
			if (chr == '\n') {
				mapofcode << str << endl;
				mapofcode << "end";
				itr++;
			}
			else if (chr == ' ') {
				mapofcode << str << endl;
				mapofcode << "space";
				itr++;
			}
			else {
				mapofcode << str << endl;
				mapofcode << chr;
				itr++;
			}
		}
	}

	//itr2 = charencode.begin();
	//for (int i = 0; i < charencode.size(); i++) {
	//	cout << itr->first << ": " << itr->second << endl; // remove the comment to see the new coding of each character
	//	itr2++;
	//}

	//cout << " distinct characters " << charencode.size() << endl // remove the comment to see how many distict characters there are

	string allcode = "";
	i = 0;
	while (file.substr(i, 1) != "\0") {
		char character = file[i];
		itr = charencode.find(character);
		allcode += itr->second;
		i++;
	}

	string copy = allcode;
	char ascii;
	ofstream compressed;
	compressed.open("compressed.txt");
	int count = 0;
	while (copy != "\0") {
		if (copy.size() < 8) {
			while (copy.size() != 8) {
				copy = '0' + copy;
				count++;
			}
		}
		ascii = char(binaryToDecimal(copy.substr(0, 8)));
		if (copy.length() > 8) {
			copy = copy.substr(8, copy.length());
		}
		else if (copy.length() == 8) {
			copy = "\0";
		}
		compressed << ascii;
	}

	cout << "------------------------------COMPRESSED------------------------------" << endl;

	return count;
}

void readCompressedFile(map<string, char>& m) {
	map<string, char>::iterator itr;
	string path;
	string file;
	ifstream filein;
	cout << "Please enter the path of the file to be read: ";
	cin >> path;
	filein.open(path);
	while (!filein) {
		cout << "ERROR!\nPlease enter the path of the file to be read: ";
		cin >> path;
		filein.open(path);
	}
	istreambuf_iterator<char> inputIt(filein), emptyInputIt;
	back_insert_iterator<string> stringInsert(file);

	copy(inputIt, emptyInputIt, stringInsert);

	ifstream codemap;
	codemap.open("mapofcode.txt");
	string str;
	string str2;
	char chr;
	while (!codemap.eof()) {
		codemap >> str;
		codemap >> str2;
		if (str2 == "end") {
			str2 = "\n";
		}
		else if (str2 == "space") {
			str2 = " ";
		}
		chr = str2[0];
		m.insert(pair<string, char>(str, chr));
	}
}

void decompress(int count) {
	map<string, char> m;
	readCompressedFile(m);
	map<string, char>::iterator itr;
	string file;
	ifstream filein;
	filein.open("compressed.txt");
	istreambuf_iterator<char> inputIt(filein), emptyInputIt;
	back_insert_iterator<string> stringInsert(file);

	copy(inputIt, emptyInputIt, stringInsert);

	int i = 0;
	string binary;
	int decimalval;
	string allcode = "";
	while (file.substr(i, 1) != "\0") {
		unsigned char current = file[i];
		decimalval = int(current);
		binary = decimalToBinary(decimalval);
		i++;
		allcode += binary;
	}
	ofstream fileout;
	fileout.open("decompressed.txt");
	bool overeight = true;
	allcode = allcode.substr(0, (allcode.length() - 8)) + allcode.substr(((allcode.length() - 8) + (count)), allcode.length());
	while (allcode != "\0") {
		bool notfound = true;
		int j = 1;
		while (notfound) {
			if (m.find(allcode.substr(0, j)) != m.end()) {
				itr = m.find(allcode.substr(0, j));
				fileout << itr->second;
				notfound = false;
				if (j != file.length()) {
					allcode = allcode.substr(j, allcode.length());
				}
				else {
					allcode = "\0";
				}
			}
			j++;
		}
	}
	cout << "------------------------------DECOMPRESSED------------------------------" << endl;
}

int main() {
	map<string, int> charcount;
	map<string, int>::iterator charitr;
	map<char, string> charencode;
	map<char, string>::iterator itr;

	string input;
	while (input != "3") {
		cout << "Please enter the number of the operation:" << endl;
		cout << "1. Compress File" << endl;
		cout << "2. Decompress File" << endl;
		cout << "3. Exit" << endl;
		cin >> input;
		if (input == "1") {
			ofstream mapofcode;
			mapofcode.open("zeroes.txt");
			int zeroestoberemoved = compress(charencode, charcount);
			mapofcode << zeroestoberemoved;
			mapofcode.close();
		}
		else if (input == "2") {
			ifstream codemap;
			codemap.open("zeroes.txt");
			string str;
			codemap >> str;
			int zeroes = stoi(str);
			decompress(zeroes);
		}
		else {
			cout << "Bye Bye :)";
		}
	}
}