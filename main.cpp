
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

//-----HUFFMAN BEGIN-------////
//para la construccion del arbol huffman
struct node
{
 node* leftChild;
 node* rightChild;
 double frequency;
 string content;
 string code;
};

vector<node> nodeArray;// Use nodeArray to record all the nodes that may be created in the whole process
string compiledHuf="", fileHuf="", textoutHuf="", arrValuesHuf[127];
int arrHuf[127];
map<string, string> tableHuf;
node huffRoot;
string stringTree;

//File Write
void writeFile(string fileName, string text) {
    ofstream file(fileName.c_str());
    file << text;
    file.close();
}

int Amount_of_characters(){
    int characters=0;
    for(int i=0;i<127;i++){
        if(arrHuf[i]>0){
            characters+=1;
        }
    }
    return characters;
}

node extractMin()//regresa el menor nodo
{
  float temp = (float) 100000000;
  vector<node>::iterator i1,pos;
  for(i1 = nodeArray.begin();i1!=nodeArray.end();i1++)
  {
    if(temp>(*i1).frequency)
    {
        pos = i1;
        temp = (*i1).frequency;
    }
 }

 node tempNode = (*pos);
 nodeArray.erase(pos);



 return tempNode;
}

node getHuffmanTree()
{
    while(!nodeArray.empty())
   {
     node * tempNode = new node;
     node * tempNode1 = new node;
     node * tempNode2 = new node;
     *tempNode1 = extractMin();
     *tempNode2 = extractMin();

     tempNode->leftChild = tempNode1;
     tempNode->rightChild = tempNode2;
     tempNode->frequency = tempNode1->frequency+tempNode2->frequency;
     nodeArray.push_back(*tempNode);
     //cout << "checkpoint2" << endl;
     if(nodeArray.size() <= 1)//only the root node exsits
     {
         //cout << "checkpoint3" << endl;
        break;
     }
    }
  return nodeArray[0];
}

void BFS(node * temproot,string s)
{
  node * root1 = new node;
  root1 = temproot;

  root1->code = s;
  if(root1 == NULL){ }
  else if(root1->leftChild == NULL && root1->rightChild == NULL)
  {
    //cout<<"the content is "<<root1->content<<endl;
    //cout<<"and its corresponding code is "<<root1->code<<endl;
    tableHuf[root1->content]=root1->code;
  }
  else
  {

    BFS(root1->leftChild,s.append("0"));
    s.erase(s.end()-1);
    BFS(root1->rightChild,s.append("1"));
    s.erase(s.end()-1);
  }
}

void getHuffmanCode()
{
  int size,i;
  double tempDouble;
  string tempString = "";

  size=Amount_of_characters();
  int x = 0;
  for(i = 0;i<size;i++)
  {
    //cout<<"letter " << i <<endl;
    node tempNode;
    //cin>>tempString;
    //cin>>tempDouble;
    while(x<127){
        x++;
        if(arrHuf[x]>0){
            //cout << "ASCII CODE TEST: " << x;
            char temp = x;
            tempString += temp;
            //cout << endl <<  tempString << " ";
            tempDouble = (double) arrHuf[x];
            //cout << tempDouble << endl;
            break;
        }

    }
    tempNode.frequency = tempDouble;
    tempNode.content = tempString;
    tempString = "";
    tempNode.leftChild = NULL;
    tempNode.rightChild = NULL;
    nodeArray.push_back(tempNode);
  }

  //cout << "checkpoint1" << endl;
  node root = getHuffmanTree();
  BFS(&root,"");
  huffRoot = root;
  //saveHuffTree(&huffRoot, fileN);
  // //saveFile();

  // cout << "Do you want to decompileHuffman?" << endl;
  // string ans;
  // cin >> ans;
  // if(ans=="y"||ans=="yes"){
  //   decompileHuffman(&root);
  // }
}


void writeCompressedHuff(string fileName){
  ofstream archivo(fileName.c_str(), std::ofstream::binary);
  for (int i = 0; i < compiledHuf.length(); ++i)
  {
    short num = compiledHuf[i];
    archivo.write((char*) &num, 2);
  }

  archivo.close();

}



void decompileHuffman(string fileName){
    
    ifstream file(fileName.c_str());
    
    int num=0;
    for (int i = 0; i < 127; ++i)
    {
      file >> num;
      if(num>0)
        arrHuf[i]=num;
    }
    //compiledHuf
    string input;
    char temp;
    if(file.is_open()){
        while(file >> noskipws >> temp){
            compiledHuf+=temp;
        }
    }
    for(int i = 0; i < input.length();i++){
        compiledHuf.append(tableHuf[string(1, input[i])]);
    }
    file.close();

    getHuffmanCode();
    node * root1 = new node;
    root1 = &huffRoot;
    //cout << "CONTENT: " << root1->content;



    for(int i = 0; i <= compiledHuf.length();i++){
      if(compiledHuf[i]!= NULL){
        if((root1->leftChild != NULL && root1->rightChild != NULL)){
            if(compiledHuf[i]=='0'){
                root1=root1->leftChild;
            }
            else{
                root1=root1->rightChild;
            }
        }else if(root1->leftChild == NULL && root1->rightChild == NULL){
            textoutHuf.append(root1->content);
            root1=&huffRoot;
            i--;
        }
      }
    }
}



void saveFileHuf(string fileN, string input){
    ofstream fileout;
    fileout.open(fileN+".huf");
    for(int i = 0; i < input.length();i++){
        compiledHuf.append(tableHuf[string(1, input[i])]);
    }
    for (int i = 0; i < 127; ++i)
    {
     fileout << arrHuf[i] << " ";
    }
    fileout << compiledHuf;
    fileout.close();
}

void Find_Values(string text){
    for(int i=0;i<text.length();i++){
        arrHuf[text[i]+0] += 1;
    }
}

void Initialize_Array(){
    for(int x=0; x<127; x++){
        arrHuf[x] = 0;
    }
}

// void loadfile(){//no recibe nombre de archivo;
//     string filename;
//     char temp;
//     cin >> filename;//FILENAME
//     filename.append(".txt");

//     ifstream fileopen;
//     fileopen.open(filename.c_str());
//     if (fileopen.is_open())
//     {
//         while (fileopen >> noskipws >> temp)
//         {
//             fileHuf+=temp;
//         }
//         fileopen.close();
//         //cout << "FULL fileHuf: " << endl << fileHuf << endl;
//     }
//     else cout << "Unable to open fileHuf";

//     Find_Values(fileHuf);
// }



//------HUFFMAN END------///

//-------LZW BEGIN------//
//File Read
string readFile(string fileName) {
    string text;
    char temp;
    
    ifstream file(fileName.c_str());
    
    string line;
    if(file.is_open()){
        while(file >> noskipws >> temp){
            text +=temp;
        }
        
    }
    file.close();
    return text;
}

//File Write Binary
template <typename Iterator>
void writeCompressedLZW(Iterator begin, Iterator end, string fileName) {      ///escribe en binario  el archivo
    ofstream archivo(fileName.c_str(), std::ofstream::binary);
    for( ; begin!=end; begin++){
        short num = *begin;
        archivo.write((char*) &num, 2);
    }

    archivo.close();
}


//File Read Binary
vector<int> readCompressedLZW(string fileName) { // lee binario el archivo y guarda los codigos de los grupos en un vector nums
    vector<int> nums;
    ifstream archivo(fileName.c_str(), std::ifstream::binary);
    
    archivo.seekg(0, archivo.end);
    long size = archivo.tellg();
    archivo.seekg(0);

    char* buffer = new char[size];

    archivo.read(buffer,size);
    
    for(int i = 0; i < size; i += 2) {
        char *charptr = &buffer[i];
        short *shortptr = (short*) charptr;
        int num = (int) (*shortptr);
        nums.push_back(num);
    }
    
    delete[] buffer;
    archivo.close();
    
    return nums;
}

//LZW
// Compress a string to a list of output symbols.
// The result will be written to the output iterator
// starting at "result"; the final iterator is returned.

//File Write LZW
void writeFileLZW(string fileName, vector<int> text) {
    ofstream file(fileName.c_str());
    for(int x = 0; x < text.size(); x++)
        file << text[x];
    file.close();
}

template <typename Iterator>
Iterator compressLZW(const string &uncompressed, Iterator result) {
 // Build the dictionary.
    int dictSize = 256;
    map<string,int> dictionary;
    for (int i = 0; i < 256; i++)
        dictionary[string(1, i)] = i;
    
    string current;
    for (string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it) {
        char next = *it;
        string pair = current + next;
        if (dictionary.count(pair))
            current = pair;
        else {
            *result++ = dictionary[current];
            // Add pair to the dictionary.
            dictionary[pair] = dictSize++;
            current = string(1, next);

        }
    }
    // Output the code for current.
    if (!current.empty())
        *result++ = dictionary[current];
    return result;
}

// Decompress a list of output ks to a string.
// "begin" and "end" must form a valid range of ints
template <typename Iterator>
string decompressLZW(Iterator begin, Iterator end) {
    // Build the dictionary.
    int dictSize = 256;
    map<int,string> dictionary;
    for (int i = 0; i < 256; i++)
        dictionary[i] = string(1, i);
    
    string current(1, *begin++);
    string result = current;
    string entry;
    for ( ; begin != end; begin++) {
        int k = *begin;
        if (dictionary.count(k))
            entry = dictionary[k];
        else if (k == dictSize)
            entry = current + current[0];
        else
            throw "Bad compressed k";
        
        result += entry;
        
        // Add current+entry[0] to the dictionary.
        dictionary[dictSize++] = current + entry[0];
        
        current = entry;
    }
    return result;
}

long fileSize(string fileName) {
    ifstream file(fileName.c_str(), std::ifstream::binary);
    
    file.seekg(0, file.end);
    long size = file.tellg();
    file.close();
    
    return size;
}

//-------LZW END------//

int main() {
    
    int op;
    string fileName, input;
    vector<int> compressed;
    vector<int> test;
    
    do {
        do {
            cout << "Elija una opcion: " << endl;
            cout << "Comprimir (1)" << endl;
            cout << "Descomprimir (2)" << endl;
            cout << "Salir (3)" << endl;
            cout << "Opcion: ";
            cin >> op;
        } while(op != 1 and op != 2 and op != 3);
        
        
        if(op == 1) {
            cout << "Nombre de archivo a comprimir: ";
            cin >> fileName;
            input = readFile(fileName + ".txt");
            long pesoTxt = fileSize(fileName + ".txt");

                //LZW
            compressLZW(input, back_inserter(compressed));
            writeFileLZW(fileName + "LZW.txt", compressed);
            writeCompressedLZW(compressed.begin(), compressed.end(), fileName + ".lzw");
            long pesoLzw = fileSize(fileName + ".lzw");
            
                //Huffman
            Initialize_Array();
                //loadfile();
            Find_Values(input);
            getHuffmanCode();
            saveFileHuf(fileName, input);
                //writeCompressedHuff(fileName+".huf");
            long pesoHuf = fileSize(fileName + ".huf");


                // -------PESOS DE ARCHIVOS---- 
            cout << "Archivo Original: " << pesoTxt << " bytes." <<endl;
            cout << "Archivo LZW: " << pesoLzw << " bytes." << endl;
            cout << "Porcentaje de compresion LZW: " << ((double)pesoLzw / pesoTxt)*100 << '%' << endl;

            cout << "Archivo Huf: " << pesoHuf << " bytes." << endl;
            cout << "Porcentaje de compresion Huf: " << ((double)pesoHuf / pesoTxt)*100 << '%' << endl;
        }
        
        else if (op == 2){
            do{
                cout << "Elija una opcion: " << endl;
                cout << "Descomprimir ambos (1)" << endl;
                cout << "Descomprimir Huf (2)" << endl;
                cin >> op;
            }while(op != 1 and op != 2);
            
            if(op == 1){
                cout << "Nombre de archivo a descomprimir: ";
                cin >> fileName;
                input = "";
                //input = readFile(fileName + ".lzw");
                //descomprime lzw
                compressed = readCompressedLZW(fileName+".lzw");

                string decompressed = decompressLZW(compressed.begin(), compressed.end());
                writeFile(fileName + "LZW.txt", decompressed);

                //descomprime huffman
                //input = readFile(fileName + ".huf");
                //readTree();
                Initialize_Array();
                decompileHuffman(fileName+".huf");
                //cout << "ASDFASDF"<<textoutHuf << "...";
                writeFile(fileName+"HUF.txt", textoutHuf);
                //decompileHuffman(fileName +"HUF.txt");
            }
            else{ //ya no se va a utilizar, lo dejamos por si acaso
                cout << "Nombre de archivo a descomprimir: ";
                cin >> fileName;
                input = readFile(fileName + ".huf");
                //node root = getHuffmanTree();
                //BFS(&root,"");
                //decompileHuffman(fileName +"HUF.txt", &huffRoot);
                
            }
        }
    } while (op != 3);
    
    return 0;
}