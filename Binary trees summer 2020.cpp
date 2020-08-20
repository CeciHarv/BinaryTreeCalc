// Copyright 2020, Bradley Peterson, Weber State University, all rights reserved. (7/20)

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cmath>
#include <cstdio>
#include <cctype>

using std::stack;
using std::istringstream;
using std::ostringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pow;

struct Node {
	string data{};
	Node* lchild{ nullptr };
	Node* rchild{ nullptr };
};

class TreeParser {
public:
	TreeParser();									//constructor
	~TreeParser();									//Destructor
	void displayParseTree();						//will display the binary tree in in-order and post-order notation
	void processExpression(string& expression);		//calls the private processExpression method
	string computeAnswer();							//calls the private computeAnswer() and returns the answer to main by grabbing the last item on mathStack and returning it.
	void inOrderTraversal() const;					//will print the nodes using inOrderTraversal by calling the private function
	void postOrderTraversal() const;				//will print the nodes using postOrderTraversal by calling the private function

protected:
	string expression{ NULL };
	int position{ NULL };

private:
	double castStrToDouble(string const& s);		//changes string to double
	string castDoubleToStr(const double d);			//changes double to string
	void initialize();								//will reinitialize an object so it can be used again.
	void processExpression(Node* p);				//bulk of processing will be here to build the tree
	void computeAnswer(Node* p);					//called by the public computeAnswer() with the root as the argument
	bool isOperator(char c) const;						//accepts char and will return true if it is + - * / or ^
	void inOrderTraversal(Node* currNode) const;	//overloaded inOrderTraversal, prints node data in in-order notation
	void postOrderTraversal(Node* currNode) const;	//overloaded postOrderTraversal, prints node data in post-order notation
	void postOrderDelete(Node* currNode) const;		//called by destructor to traverse and delete nodes in post-Order;

	stack<string> mathStack;
	Node* root{ nullptr };

};

void TreeParser::initialize() {
	expression = "";
	position = 0;
	while (!mathStack.empty()) {
		mathStack.pop();
	}
}

void TreeParser::processExpression(Node* p) {		//private
	while (position < expression.length()) {		//while we are checking the chars in the expression
		if (expression[position] == '(') {			//if the char is the beginning of a parenthesis
			Node* newNode = new Node;				//create a node and set the pointer to nullptr
			newNode->lchild = nullptr;
			newNode->rchild = nullptr;
			if (p->lchild) {						//if there's already a lchild, then put it on the right
				p->rchild = newNode;
			}
			else {
				p->lchild = newNode;				//put it on the left of the current node
			}
			position++;								//increment position
			processExpression(newNode);
		}
		else if (expression[position] == ')') {		//if the char is the ending of a parenthesis
			position++;
			return;
		}
		else if (isdigit(expression[position]) || expression[position] == '.') {		//if the char is a digit
			while (isdigit(expression[position]) || expression[position] == '.') {
				p->data += expression[position];
				position++;	
			}
			return;
		}
		else if (isOperator(expression[position])) {//if the char is an operator 
			p->data = expression[position];			//set p data to operator at position
			position++;
			Node* newNode = new Node;
			p->rchild = newNode;
			processExpression(newNode);
		}
		else {
			position++;
		}
	}
}

void TreeParser::processExpression(string& expression) {	//public
	if (expression.length() > 0) {					//check to verify the expression isn't empty
		this->expression = expression;				//set member object to the expression passed in
		this->position = 0;										
		this->root = new Node;						//create new node and have root point to that node
		processExpression(this->root);				//call private processExpression
	}
}

void TreeParser::computeAnswer(Node* p) {			//private
	double a;
	double b;
	string result;

	if (p) {										//do this while there are still nodes on the tree that haven't been traversed
		computeAnswer(p->lchild);
		computeAnswer(p->rchild);		
		if (isdigit(p->data[0])) {					//if is number
			mathStack.push(p->data);				//push node->data onto the mathStack
		}
		else {//if it's an operator
			b = castStrToDouble(mathStack.top());	//pop the last two pieces off the stack into temp variables
			mathStack.pop();
			a = castStrToDouble(mathStack.top());
			mathStack.pop();
			if (p->data == "+") {
				mathStack.push(castDoubleToStr(a + b));	//do math operation with the operator and push result back onto the stack as a string				
			}			
			else if (p->data == "-") {
				mathStack.push(castDoubleToStr(a - b));	//do math operation with the operator and push result back onto the stack as a string				
			}
			else if (p->data == "*") {
				mathStack.push(castDoubleToStr(a * b));	//do math operation with the operator and push result back onto the stack as a string				
			}
			else if (p->data == "/") {
				mathStack.push(castDoubleToStr(a / b));	//do math operation with the operator and push result back onto the stack as a string				
			}
			else {
				mathStack.push(castDoubleToStr(pow(a, b)));	//do math operation with the operator and push result back onto the stack as a string
			}
		}
	}
}

string TreeParser::computeAnswer() {				//public
	
	string result;
	computeAnswer(this->root);
	result = mathStack.top();						//cast result to string and return it
	mathStack.pop();
	return result;	
}

bool TreeParser::isOperator(char c) const {
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
		return true;
	}

	return false;
}

void TreeParser::inOrderTraversal() const {
	inOrderTraversal(root);							//call overloaded inOrderTraversal starting with the root as the argument
	cout << endl;
}

void TreeParser::inOrderTraversal(Node* currNode) const {
	if (currNode) {									//if there's a node there
		inOrderTraversal(currNode->lchild);			//traverser left, and keep doing it until you can't any more
		cout << currNode->data << ' ';				//print out the data at this node
		inOrderTraversal(currNode->rchild);			//check for right child and keep going
	}
}

void TreeParser::postOrderTraversal() const {
	postOrderTraversal(root);
	cout << endl;
}

void TreeParser::postOrderTraversal(Node* currNode) const {
	if (currNode) {
		postOrderTraversal(currNode->lchild);
		postOrderTraversal(currNode->rchild);
		cout << currNode->data << " ";
	}
}

void TreeParser::postOrderDelete(Node* currNode) const {
	if (currNode) {
		postOrderDelete(currNode->lchild);
		postOrderDelete(currNode->rchild);
		delete currNode;
	}
}

double TreeParser::castStrToDouble(const string &s) {
	istringstream iss(s);
	double x;
	iss >> x;
	return x;
} 

string TreeParser::castDoubleToStr(const double d) {
	ostringstream oss;
	oss << d;
	return oss.str();

} 

TreeParser::TreeParser() {
	initialize();
}

TreeParser::~TreeParser() {
	postOrderDelete(root);												
}

void TreeParser::displayParseTree() {
	cout << endl;
	cout << "The expression seen using in-order traversal: "; 
	inOrderTraversal();
	cout << "The expression seen using post-order traversal: "; 
	postOrderTraversal();	
}


void pressEnterToContinue() {
	cout << endl;
	printf("Press Enter to continue\n");
	std::cin.get();

}

// Copyright 2019, Bradley Peterson, Weber State University, all rights reserved. (07/2020)

int main() {


	TreeParser *tp = new TreeParser;
	
	string expression = "(4+7)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 11 as a double output
		
	expression = "(7-4)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 3 as a double output
	
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 45 as a double output
	
	expression = "(4^3)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 64 as a double output
	
	expression = "((2-5)-5)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display -8 as a double output
	
	expression = "(5*(6/2))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 15 as a double output
	
	expression = "((1 + 2) * (3 + 4))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 21 as a double output
	
	expression = "(543+321)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 864 as a double output
	
	expression = "(7.5-3.25)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 4.25 as a double output
	
	expression = "(5 + (34 - (7 * (32 / (16 * 0.5)))))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 11 as a double output
	
	expression = "((5*(3+2))+(7*(4+6)))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 95 as a double output
	
	expression = "(((((3+12)-7)*120)/(2+3))^3)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display close to 7077888 as a double output 
		
	expression = "(((((9+(2*(110-(30/2))))*8)+1000)/2)+(((3^4)+1)/2))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display close to 1337 as a double/decimal output
		
	pressEnterToContinue();
	delete tp;
	return 0;
}
