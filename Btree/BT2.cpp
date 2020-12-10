#include <vector>
#include <stack>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

class Node{
public:
  bool full;      // fullflag
  int nodeSize;   // nodeSize
  int n;          // 현재 노드안의 key개수
  bool overflow;  // overflow flag
  int idx;        // child idx

  vector<int> key;  // key data
  vector<Node*> child;  // child pointer


  Node(int m)
  {
    nodeSize = m;
    overflow = false;
    n = 0;
    idx = 1;
    key.resize(m,0);
    child.resize(m,0);
  }

  void printNode();
  void insertKey(int);
  void sortKey();
  bool isLeaf();
  void copy(Node* , int , Node*);
  Node* findSibling(Node*, int *);
  Node* backBuffer();
  int getIdx();
  void deleteToMerge(int);
  int getMid()
  {
    return key[nodeSize/2];
  }
  void deleteKey(int delete_key);
  friend class BT;
};


class BT{
private:
  Node* root; // 트리의 root
  int M;      // m원 트리
  int MAX;
  int MIN;
  bool isLeft;

public:
  BT(int size)
  {
    root = new Node(size);
    M = size;
    MAX = size-1;
    MIN = ceil(float(size)/2)-1;
    isLeft = false;
  }
  void insert(int);
  void traverse(Node*);
  void deleteBT(int);
  int searchLeft(Node*);
  int searchRight(Node*);
  Node* findBS(Node*, Node*);
  Node* getRoot()
  {
    return root;
  }
};

void Node::deleteKey(int delete_key) {

	int i = 1;
	while (i <= n && delete_key > key[i] ) {
		i++;
	}


	if (delete_key == key[i]) {
		for (; i < n; i++) {
			key[i] = key[i+1];
		}
		key[i] = 0;
	}

	n--;

	// 이 함수는 단말 노드에서만 사용되므로 포인터를 옮길 걱정이 없다.

}

void Node::deleteToMerge(int intermediate) {
	for (size_t i = intermediate; i < n; i++)
	{
		key[i] = key[i + 1];
		child[i] = child[i + 1];
	}

	child[n] = NULL;
	key[n] = 0;
	n--;
}

void Node::printNode()
{
  vector<int>::iterator it;

  for(it = key.begin(); it != key.end(); it++)
  {
    cout << *it << ' ';
  }
  cout << endl <<"n : " << n << endl;
  //cout << "printNode() end" << endl;
}

bool Node::isLeaf()
{
  vector<Node*>::iterator it;

  for(it = child.begin(); it!=child.end();it++)
  {
    if(*it == NULL)
    {
      continue;
    }
    else
    {
      return false;
    }
  }
  return true;
}

void Node::insertKey(int a)
{
  int i = 1;
  vector<int>::iterator it = key.begin(); // key vector의 시작점

  // 들어갈 key값 비교
  while(i <= n && a > key[i])
  {
    i++;
  }

  for(int j = 0; j<i;j++)
  {
    it++;
  }
  key.insert(it,a);

  for(int j = n+1;j>i;j--)
  {
    child[j] = child[j-1];
  }

  idx = i;
  n++;

  return;
}

int BT::searchLeft(Node* dp)
{
  dp = dp->child[dp->n - 1];
  while(1)
  {
    if(dp->isLeaf())
      break;
    dp=dp->child[dp->n];
  }
  return dp->n;

}


int BT::searchRight(Node* dp)
{
  dp = dp->child[dp->n];
  while(1)
  {
    if(dp->isLeaf())
      break;
    dp=dp->child[0];
  }
  return dp->n;

}

void Node::sortKey()
{
  int temp;

  // node 구조상 내림차순 정렬 후 0을 제외한 오름차순 정렬
  for(int i = 1;i<nodeSize-1 ;i++)
  {
    for(int j = i+1; j<nodeSize;j++)
    {
      if(key[i] < key[j])
      {
        temp = key[i];
        key[i] = key[j];
        key[j] = temp;
      }
    }
  }

  for(int i = 1; i< nodeSize-1; i++)
  {
    for(int j = i+1;j<nodeSize;j++)
    {
      if(key[i]==0 || key[j]==0)
      {
        break;
      }
      else if(key[i] > key[j])
      {
        temp = key[i];
        key[i] = key[j];
        key[j] = temp;
      }
    }
  }
}

Node* Node::findSibling(Node* under,int *parentIdx)
{
  //scout << "여기서 세그 ? " << endl;
  int idx = 0;
  for(int i = 0; i < nodeSize; i++)
  {
    if(!(under == child[i]))
    {
      idx++;
    }
    else
      break;
  }
  //cout << "under의 idx 는 " << idx << endl;
  int max = 0;
  int maxIdx = 0;
  Node *best = NULL;

  for(int j = idx - 1; j <= idx+1;j++)
  {
    //cout << "for문은 들어가나?" << endl;
    //cout << "j :" << j << endl;
    if(j >= nodeSize) break;
    if(j == idx) continue;
    if(j < 0) continue;
    //child[j] -> printNode();
    if(child[j] != NULL)
    {
      if(child[j]->n > ceil(float(nodeSize)/2) || max < child[j]->n)
      {
        maxIdx = j;
        max = child[j]->n;
        best = child[j];
      }
    }
  }
  cout << "max : " << maxIdx << endl;
  *parentIdx = maxIdx;
  //cout << "seg?" << endl;
  //best->printNode();
  return best;
}

void Node::copy(Node* sib, int par, Node *under)
{
  int i,j;
  sib->printNode();
  for(i = 1; i<=sib->n;i++)
  {
    cout << sib->key[i] << ' ' ;
    key[i] = sib->key[i];
    child[i-1] = sib->child[i-1];
    n++;
  }
  cout << endl;

  child[i-1] = sib->child[i-1];

  key[i++] = par;
  n++;
  for(j = 1; j <= under->n;j++)
  {
    key[i] = under->key[j];
    child[i-1] = under->child[j-1];
    n++;
  }
  child[i-1] = under->child[j-1];

}

Node* BT::findBS(Node* x, Node* parent) {
	Node* bsNode = NULL;
	int underflowLine = ceil(M / 2 + M % 2) - 1;
	bool keyDis = true;



	int i = 0;
	for (i = 0; i < parent->n; i++)
	{
		if (parent->child[i] == x) {
			break;
		}
	}

	if (i == 0) {

		bsNode = parent->child[1];
		isLeft = false;
	}
	else if (i == parent->n) {
		 bsNode = parent->child[parent->n-1];
		 isLeft = true;
	}
	else {

		Node* temp1 = parent->child[i - 1];
		Node* temp2 = parent->child[i + 1];

		if (temp1->n > temp2->n) {
			bsNode = temp1;
			isLeft = true;
		}
		else {
			bsNode = temp2;
			isLeft = false;
		}
	}

	return bsNode;
}

void BT::deleteBT(int key) {
	Node* x = root;
	stack<Node*> stk;
	Node* y = x;
	Node* bsNode = NULL;
	Node* old_x = NULL;


	int i;
	do {
		i = 1;
		while (i <= x->n && key > x->key[i]) {
			i++;
		}

		if (i <= x->n && key == x->key[i]) {// 삭제할 키를 발견함
			break;
		}
		stk.push(x);
	} while ((x = x->child[i - 1]) != NULL);

	if (x == NULL) { return; } // 삭제할 키 발견 못함 트리 내부에 없음.

	if (x->child[0] != NULL) { // internal node

		Node* inter = x;
		stk.push(x);


    int leftKey = searchLeft(x);
    int rightKey = searchRight(x);

    if(rightKey > leftKey){
      x = x->child[i]; // x노드의 오른쪽 서브트리
  		do {
  			stk.push(x);
  		} while ((x = x->child[0]) != NULL);  // 왼쪽 끝으로 내려간다    ----> 오른쪽 서브트리에서 가장 작은 값을 찾는다.
    }
	else {
		x = x->child[i - 1];
		do {
			stk.push(x);
		} while ((x = x->child[x->n]) != NULL);
	}


		if (x == NULL) {
			x = stk.top();
			stk.pop();

			int temp = inter->key[i];
			if (rightKey > leftKey) {

				inter->key[i] = x->key[1];
				x->key[1] = temp;  // 후행키와 키값을 교환한다.
			}
			else {
				inter->key[i] = x->key[x->n];
				x->key[x->n] = temp;  // 후행키와 키값을 교환한다.
			}

		}
	}

	bool finished = false;
	x->deleteKey(key);

	if(!stk.empty()){
		y = stk.top();
		stk.pop();
	}

	do {
		if (x == y || !(x->n < MIN)) {
			finished = true;

		}
		else {

			bsNode = findBS(x, y);


			int m = 0;
			if (isLeft) {
				while (m <= y->n && !(y->child[m] == bsNode && y->child[m+1] == x)) {
					m++;
				}
			}
			else {
				while (m <= y->n && !(y->child[m] == x && y->child[m+1] == bsNode)) {
					m++;
				}
			}
			m++;
			int intermediate_y = y->key[m];


			/*
			키 재분배
      cout << "재분배 start" << endl;
			*/

			if (bsNode->n - 1 >= MIN) {
				Node* tempNode = new Node(ceil(M*1.5) + 1);



				if (isLeft) {
					int i;
					for (i = 1; i < bsNode->n + 1; i++) { // copy bsNode
						tempNode->key[i] = bsNode->key[i];
						tempNode->child[i - 1] = bsNode->child[i - 1];
					}
					tempNode->child[i - 1] = bsNode->child[i - 1];

					tempNode->key[i] = intermediate_y;  // copy key_y
					i++;
					int j;
					for (j = 0; j < x->n; j++) {  // copy x node
						tempNode->key[i] = x->key[j+1];
						tempNode->child[i - 1] = x->child[j];
						i++;
					}
					tempNode->child[i - 1] = x->child[j];
				}
				else {
					int i;
					for (i = 1; i < x->n; i++) {  // copy x node
						tempNode->key[i] = x->key[i];
						tempNode->child[i - 1] = x->child[i - 1];
					}
					tempNode->child[i - 1] = x->child[i - 1];

					tempNode->key[i] = intermediate_y;  // copy key_y
					i++;

					int j;
					for (j = 0; j < bsNode->n; j++) { // copy bsNode
						tempNode->key[i] = bsNode->key[j+1];
						tempNode->child[i - 1] = bsNode->child[j];
						i++;
					}
					tempNode->child[i - 1] = bsNode->child[j];
				}


				if (isLeft) {
					bsNode->key.clear();
					bsNode->key.resize(bsNode->nodeSize);
					bsNode->child.clear();
					bsNode->child.resize(bsNode->nodeSize);
					int old_n = bsNode->n;
					bsNode->n = 0;


					int i;
					for (i = 0; i < old_n - 1; i++)
					{
						bsNode->key[i + 1] = tempNode->key[i + 1];
						bsNode->child[i] = tempNode->child[i];
						bsNode->n++;
					}
					bsNode->child[i] = tempNode->child[i];

					y->key[m] = tempNode->key[i + 1];
					i++;

					x->key.clear();
					x->key.resize(x->nodeSize);
					x->child.clear();
					x->child.resize(x->nodeSize);
					old_n = x->n;
					x->n = 0;

					int j;
					for (j = 0; j < old_n + 1; j++)
					{
						x->key[j + 1] = tempNode->key[i + 1];
						x->child[j] = tempNode->child[i];
						x->n++;
						i++;
					}
					x->child[j] = tempNode->child[i];

				}
				else {
					x->key.clear();
					x->key.resize(x->nodeSize);
					x->child.clear();
					x->child.resize(x->nodeSize);
					int old_n = x->n;
					x->n = 0;

					int i;
					for (i = 0; i < old_n + 1; i++)
					{
						x->key[i + 1] = tempNode->key[i + 1];
						x->child[i] = tempNode->child[i];
						x->n++;

					}
					x->child[i] = tempNode->child[i];

					y->key[m] = tempNode->key[i + 1];
					i++;

					bsNode->key.clear();
					bsNode->key.resize(bsNode->nodeSize);
					bsNode->child.clear();
					bsNode->child.resize(bsNode->nodeSize);
					old_n = bsNode->n;
					bsNode->n = 0;

					int j;
					for (j = 0; j < old_n - 1; j++)
					{
						bsNode->key[j + 1] = tempNode->key[i + 1];
						bsNode->child[j] = tempNode->child[i];
						bsNode->n++;
						i++;
					}
					bsNode->child[j] = tempNode->child[i];


				}

				finished = true;

			}

			/*
			노드 합병
      cout << "합병 start" << endl;
			*/

			else {


				if (isLeft) {
					int i = bsNode->n;
					i++;

					bsNode->key[i] = intermediate_y;
					bsNode->n++;
					i++;

					int j;
					for (j = 0; j < x->n; j++)
					{
						bsNode->key[i] = x->key[j + 1];
						bsNode->child[i - 1] = x->child[j];
						bsNode->n++;
						i++;
					}
					bsNode->child[i - 1] = x->child[j];

					y->deleteToMerge(m);

				}
				else {
					int i = x->n;
					i++;

					x->key[i] = intermediate_y;
					x->n++;
					i++;

					int j;
					for (j = 0; j < bsNode->n; j++)
					{
						x->key[i] = bsNode->key[j + 1];
						x->child[i - 1] = bsNode->child[j];
						x->n++;
						i++;
					}
					x->child[i - 1] = bsNode->child[j];

					y->deleteToMerge(m);
				}

				old_x = x;
				x = y;




				if (!stk.empty()) {
					y = stk.top();
					stk.pop();
				}
				else {
					finished = true;
				}
			}
		}


	}while (!finished);

	if (y == NULL || y->n == 0) {
		if (isLeft) {
			if (bsNode != NULL) {
				root = bsNode;
			}
			else {
				root = old_x;
			}
		}
		else {
			root = old_x;
		}
		delete y;
	}

}







void BT::insert(int key)
{
  Node *p = root;       //root 포인터
  Node *x = new Node(M);
  Node *y = NULL;

  stack<Node*> childs;
  bool isFull = false;  // full check flag
  bool finish = false;  // finish flag
  int i;
  // root가 비어 있을 경우
  if(p->n == 0)
  {
    p->insertKey(key);
    return;
  }
  // PPT 상의 do while문으로 구현
  do{
    i = 1;

    while(i <= p->n && key > p->key[i])
    {
      i++;
    }

    if(i<=p->n && key == p->key[i])
    {
      cout << "동일값 예외처리" << endl;
      return;
    }
    else
    {
      childs.push(p);
    }
  }while((p=p->child[i-1]) != NULL);

  // stack에서 하나씩 빼면서 key를 넣을 공간 탐색
  p = childs.top();
  childs.pop();

  do{
    if(p->n < M-1)
    {
      p->insertKey(key);
      finish = true;

      if(isFull){
        int j = p->getIdx();
        p->child[j] = y;
      }
    }
    else
    {
      // overflow
      Node *temp = new Node(p->nodeSize+1);

      temp->key = p->key;

      for(int i = 0; i< p->nodeSize;i++)
      {
        temp->child[i] = p->child[i];
      }

      temp->n = p->n;
      temp->insertKey(key);

      key = temp->getMid();
      int j = temp->getIdx();

      if(isFull)
      {
        Node* Y = y;
        temp->child[j] = Y;
      }

      // 중요한 과정.. 안하면 잘 안됨..
      p->key.clear();
      p->key.resize(p->nodeSize);
      p->child.clear();
      p->child.resize(p->nodeSize);
      p->n = 0;

      int k;
      for(k = 1; k<temp->nodeSize/2;k++)
      {
        p->key[k] = temp->key[k];
        p->child[k-1] = temp->child[k-1];
        p->n++;
      }
      p->child[k-1] = temp->child[k-1];

      y = new Node(M);
      y = temp->backBuffer();

      isFull = true;
    }

    if(!childs.empty())
    {
      p=childs.top();
      childs.pop();
    }
    // 높이가 증가할 경우
    else if(!finish)
    {
      Node *newHeight = new Node(M);
      newHeight->key[1] = key;
      newHeight->n = 1;
      newHeight->child[0] = p;
      newHeight->child[1] = y;
      root = newHeight;
      finish = true;
    }
  }while(!finish);



}

// void BT::deleteBT(int key)
// {
//   Node *dp = root;
//   Node *temp,*sibling;
//   stack<Node*> stk;
//   int i,left,right;
//   bool finish = false;
//
//   do{
//     i = 1;
//
//     while(i <= dp->n && key > dp->key[i])
//     {
//       i++;
//     }
//
//     if(i<=dp->n && key == dp->key[i])
//     {
//       break;
//     }
//     else
//     {
//       stk.push(dp);
//     }
//   }while((dp=dp->child[i-1]) != NULL);
//
//   if(!dp)
//   {
//     cout << "Can't find the key to delete" << endl;
//     return;
//   }
//   // temp = stk.top();
//   // temp->printNode();
//   // cout << "30의 노드" << endl;
//   //dp->printNode();
//   temp = dp;
//   if(!dp->isLeaf())
//   {
//     //temp = dp;  // 추가 탐색을 위한 포인터
//     left = searchLeft(temp,i);
//     right = searchRight(temp,i);
//
//     if(left < right)
//     {
//       stk.push(temp);
//       temp = temp->child[temp->n];
//       stk.push(temp);
//       while(1)
//       {
//         if(temp->isLeaf())
//           break;
//         temp=temp->child[0];
//         stk.push(temp);
//       }
//     }
//     else
//     {
//       stk.push(temp);
//       temp = temp->child[temp->n - 1];
//       stk.push(temp);
//       while(1)
//       {
//         if(temp->isLeaf())
//           break;
//         temp=temp->child[temp->n];
//         stk.push(temp);
//       }
//     }
//
//     cout << "temp Data" << endl;
//     temp->printNode();
//     // 후행키 사용하여 swap 및 delete
//     if(!dp->isLeaf())
//     {
//       int swap = dp->key[i];
//
//       if(right>left){
//         cout << "Go Right "  << endl;
//         dp->key[i] = temp->key[1];
//         temp->key[1] = 0;
//         temp->n--;
//         //sorting 해야됨
//         temp->sortKey();
//       }
//       else
//       {
//         cout << "Go Left "  << endl;
//         dp->key[i] = temp->key[temp->n];
//         temp->key[temp->n--] = 0;
//         temp->sortKey();
//         //dp->printNode();
//         //temp->printNode();
//       }
//     }
//
//   }// leaf 노드가 아닐 시 key값 교환 끝
//   else  // leaf 노드 일때
//   {
//     temp->key[i] = 0;
//     temp->n--;
//     temp->sortKey();
//   }
//
//   cout << "dp print" << endl;
//   //dp->sortKey();
//   dp->printNode();
//
//   cout << "temp print" << endl;
//   temp->printNode();
//
//
//
//
//
//
//
// stk.pop();
// Node *parent;
// Node *bigNode = new Node(2*M);
// int parentIdx = 0;
//
//   do{
//     // 후행키 사용
//     if(temp->n < MIN)
//     {
//       cout << "underflow" << endl;
//       parent = stk.top();
//       stk.pop();
//       sibling = parent->findSibling(temp,&parentIdx);
//       cout << "parent Idx : " << parentIdx << endl;
//       if(sibling)
//       {
//         cout << "key 재분배" << endl;
//         // Make bigNode
//         bigNode->copy(sibling,parent->key[parentIdx+1],temp);
//         bigNode->printNode();
//
//       }
//       else
//       {
//         cout << "key 병합" << endl;
//       }
//
//       temp = stk.top();
//       temp = stk.pop();
//     }
//     else
//     {
//       finish = true;
//     }
//     // if(temp->n - 1 == 0)
//     // {
//     //   cout << "dp key 변환" << endl;
//     //   dp->key[i] = temp->key[temp->n];
//     //   dp->printNode();
//     //   stk.pop();
//     //   temp = stk.top();
//     //   cout << "sibling의 노드" << endl;
//     //   temp->printNode();
//     //   finish = true;
//     // }
//   }while(!finish);
//
// }

// MID값의 뒷부분을 리턴
Node* Node::backBuffer(){
  Node* back = new Node(nodeSize-1);

  int j = 1;
  int i = 0;
  for (i = nodeSize/2+1; i < nodeSize; i++) {
	  back->key[j] = key[i];
    back->child[j-1] = child[i-1];
	  back->n++;
	  j++;

  }
  back->child[j - 1] = child[i - 1];

  return back;
}


int Node::getIdx(){
  int i = idx;
  idx = 1;  // idx 초기화
  return i;
}

// 중위 순회 알고리즘으로 traverse 함수 구현
void BT::traverse(Node* root){

  if(root == NULL){
    return;
  }
  else{
	  int i;
    for ( i = 0; i < root->n; i++) {
      traverse(root->child[i]);
      if(i != M){
        cout << root->key[i+1] << ' ';
      }
    }
	if (i < M) {
		traverse(root->child[i]);
	}
  }
}

int main()
{
  int temp;
  ifstream in("data.txt");  // 데이터량이 많기에 파일 데이터로 읽어들임
  ifstream out("data2.txt");
  BT bt3(3);  // 3원 B 트리
  BT bt4(4);  // 4원 B 트리

  vector<int> data;
  vector<int> data2;
  data.resize(40);
  data2.resize(40);

  for(int i =0 ; i<40;i++)
  {
      in >> data[i];
  }

  for(int i = 0; i<40;i++)
  {
    out >> data2[i];
  }

cout <<  "<<<< 3원 B 트리 삽입 >>>>" << endl;
    for (int i = 0; i < 40; i++) {

      bt3.insert(data[i]);
      bt3.traverse(bt3.getRoot());
      cout << endl;

    }

cout <<  "<<<< 3원 B 트리 삭제 >>>>" << endl;
    for (int i = 0; i < 40; i++) {

      bt3.deleteBT(data2[i]);
      bt3.traverse(bt3.getRoot());
      cout << endl;

        }

cout <<  "<<<< 4원 B 트리 삽입 >>>>" << endl;
    for (int i = 0; i < 40; i++) {

      bt4.insert(data[i]);
      bt4.traverse(bt4.getRoot());
      cout << endl;
    }

        cout <<  "<<<< 4원 B 트리 삭제 >>>>" << endl;
            for (int i = 0; i < 40; i++) {

              bt4.deleteBT(data2[i]);
              bt4.traverse(bt4.getRoot());
              cout << endl;

                }
}
