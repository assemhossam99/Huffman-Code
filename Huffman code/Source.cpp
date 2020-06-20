#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <fstream>

using namespace std;

map<char, int> freqOfCharacter;
map<char, string> mp;
vector<char> decoded;
struct node
{
	char c;
	int freq;
	node *left, *right;
};

struct comp
{
	bool operator() (node *l, node *r)
	{ 
		return l->freq > r->freq;
	}
};

node *setNode(char c, int freq, node *l, node *r)
{
	node *newNode = new node();
	newNode->c = c;
	newNode->freq = freq;
	newNode->left = l;
	newNode->right = r;
	return newNode;
}

void encode(node *root, string s)
{
	if (root == nullptr)
		return;
	if (!root->left && !root->right)
		mp[root->c] = s;

	encode(root->left, s + '0');
	encode(root->right, s + '1');
}

void decode(node *root, int &idx, string s)
{
	if (root == nullptr)
		return;
	if (!root->left && !root->right)
	{
		decoded.push_back(root->c);
		return;
	}
	idx++;
	if (s[idx] == '0')
		decode(root->left, idx, s);
	else
		decode(root->right, idx, s);
}

void huffman(string txt)
{
	priority_queue<node*, vector<node*>, comp> pq;
	for (auto it : freqOfCharacter)
	{
		node *newNode = setNode(it.first, it.second, nullptr, nullptr);
		pq.push(newNode);
	}

	while (pq.size() > 1)
	{
		node *leftNode = pq.top();
		pq.pop();
		node *rightNode = pq.top();
		pq.pop();
		node *newNode = setNode('\0', leftNode->freq + rightNode->freq, leftNode, rightNode);
		pq.push(newNode);
	}
	node *root = pq.top();
	string str = "";
	encode(root, str);
	for (int i = 0; i < txt.size(); i++)
		str += mp[txt[i]];
	int id = -1;
	while (id < (int)str.size() - 2)
		decode(root, id, str);
}

int averageNumberOfBits(string s)
{
	int ans = 0;
	for (auto it : mp)
		ans += it.second.size() * freqOfCharacter[it.first];
	return ans / s.size();
}

int main()
{
	string  x, encoded = "";
	string txt = "";
	ifstream inFile;
	inFile.open("test5.txt");
	if (!inFile)
	{
		cout << "Unable to open the file";
		exit(0);
	}
	while (getline(inFile, x))
	{
		txt += x;
	}
	inFile.close();

	for (int i = 0; i < txt.size(); i++)
		freqOfCharacter[txt[i]]++;
	cout << "The original tesxt is: " << txt << endl;
	huffman(txt);
	cout << "The codes of the characters are: " << endl;
	for (auto it : mp)
		cout << it.first << ' ' << it.second << endl;
	cout << "The encoded text is: ";
	for (int i = 0; i < txt.size(); i++)
		cout << mp[txt[i]];
	cout << endl;
	cout << "The decoded string is: ";
	for (int i = 0; i < decoded.size(); i++)
		cout << decoded[i];
	cout << endl;
	cout << "The average numer of bits is: " << averageNumberOfBits(txt) << endl;

	system("pause");
	return 0;
}