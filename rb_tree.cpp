/*
 * rb_tree.cpp
 *
 *  Created on: 2016��9��24��
 *      Author: HongXu
 */
#include<iostream>
#include<deque>
#include<time.h>
#include<stdlib.h>
using namespace std;
#define B 0
#define R 1


/*
 * �ڵ�ṹ
 */
struct Node{

	int val, color;
	Node *right, *left, *p;

	Node(int v = 0, int c = 1) : val(v), color(c), right(NULL), left(NULL), p(NULL){}
};


/*
 * ʵ��ִ���жϵĺ���
 */
bool do_judge(Node* cur, int count, int& fuck){
	if(cur == NULL){
		if(fuck == -1) fuck = count;
		return fuck == count ? true : false;
	}
	if(cur->color == R && (cur->p == NULL || cur->p->color == R)) {cout << "!!" << endl;return false;}
	if(cur->color == B) ++count;
	if(!do_judge(cur->left, count, fuck)) return false;
	if(!do_judge(cur->right, count, fuck)) return false;
	return true;
}

/*
 * �ж��Ƿ�Ϊ��������������Ϊ���ڵ㡣
 */
bool is_BRtree(Node* root){
	int fuck = -1;
	return do_judge(root, 0, fuck);
}


/*
 * �������
 */
class RB_tree{
public:
	Node* root;
public:
	RB_tree() : root(NULL){}
	Node* find(int val);
	void insert(int val);
	void print(Node* cur);
	void print();
	void lzhuan(Node** cur);
	void rzhuan(Node** cur);
	void remove(int val);

};

/*
 * ����ת����
 */
void RB_tree::lzhuan(Node** c){
	if(*c == NULL) return;
	Node *cur = *c, *pre = cur->p, *ppre = pre->p;
	pre->right = cur->left;
	if(cur->left) cur->left->p = pre;
	cur->left = pre;
	pre->p = cur;
	cur->p = ppre;
	if(ppre == NULL) root = cur;
	else ppre->left == pre ? ppre->left = cur : ppre->right = cur;
}

/*
 * ����ת����
 */
void RB_tree::rzhuan(Node** c){
	if(*c == NULL) return;
	Node *cur = *c, *pre = cur->p, *ppre = pre->p;
	pre->left = cur->right;
	if(cur->right) cur->right->p = pre;
	cur->right = pre;
	pre->p = cur;
	cur->p = ppre;
	if(ppre == NULL) root = cur;
	else ppre->left == pre ? ppre->left = cur : ppre->right = cur;
}


/*
 * ��ӡ�����
 */
void RB_tree::print(){
	print(root);
}

/*
 * ʵ��ִ�д�ӡ������ĺ��������˺������أ�
 */
void RB_tree::print(Node* cur){
	if(cur == NULL) return;
	print(cur->left);
	cout << cur->val << ", ";
	print(cur->right);
}



/*
 * �ں�������ҵ�ĳһ��ֵ����Ӧ�Ľڵ㣬������û�������Ľڵ��򷵻����������ĸ��ڵ㡣
 */
Node* RB_tree::find(int val){
	Node* cur = root;
	while(cur != NULL){
		if(cur->val == val) return cur;
		else if(cur->val > val){
			if(cur->left == NULL) return cur;
			else cur = cur->left;
		}
		else if(cur->right == NULL) return cur;
		else cur = cur->right;
	}
	return cur;
}

/*
 * ���뺯��
 */
void RB_tree::insert(int val){
	Node* pre = find(val), *cur = new Node(val), *ppre = NULL;
	if(pre == NULL) root = cur;
	else if(pre->val == val) return;
	else if(pre->val > val) pre->left = cur;
	else pre->right = cur;
	cur->p = pre;
	while(cur->p != NULL){
		pre = cur->p, ppre = pre->p;
		if(pre->color == B || cur == root) break;
		if(ppre->left && ppre->right && ppre->left->color == R && ppre->right->color == R){
			ppre->left->color = ppre->right->color = B;
			ppre->color = R;
			cur = ppre;

		}
		else if(ppre->left == pre){
			if(pre->right == cur){
				lzhuan(&cur);
				swap(cur, pre);
			}
			rzhuan(&pre);
			ppre->color = R;
			pre->color = B;
			break;
		}
		else if(ppre->right == pre){
			if(pre->left == cur){
				rzhuan(&cur);
				swap(cur, pre);
			}
			lzhuan(&pre);
			ppre->color = R;
			pre->color = B;
			break;
		}
	}
	if(root->color == R) root->color = B;
}

/*
 * ɾ������
 */
void RB_tree::remove(int val){
	Node* cur = find(val), *tmp = cur;//�ҵ�Ҫɾ���Ľڵ�
	if(cur == NULL || cur->val != val) return;


	//��ɾ����ͨ������һ������ֱ��ɾ����Ҫ�ҵ�����������С�ڵ�������������ڵ��Ҫɾ���Ľڵ㽻��Ȼ����ɾ��
	if(cur->right != NULL){
		tmp = cur->right;
		while(tmp->left != NULL) tmp = tmp->left;
	}
	swap(tmp->val, cur->val);
	Node* pre = tmp->p;

	if(pre == NULL) tmp == cur ? root = tmp->left : root = tmp->right;
	else {
		if(tmp != cur)
			pre->left == tmp ? pre->left = tmp->right : pre->right = tmp->right;
		else pre->left == tmp ? pre->left = tmp->left : pre->right = tmp->left;
	}

	tmp == cur ? cur = tmp->left : cur = tmp->right;
	if(cur != NULL) cur->p = pre;
	if(tmp->color == R || pre == NULL) {
		delete tmp;
		return;
	} else {
		delete tmp;
		tmp = NULL;
	}
	while(cur != root){
		if(cur != NULL){
			pre = cur->p;
			if(cur->color == R) {
				cur->color = B;
				return;
			}
		}
		Node* brother = pre->left == cur ? pre->right : pre->left;
		if(brother->color == B && (brother->left == NULL || brother->left->color == B) && (brother->right == NULL || brother->right->color == B)){
			brother->color = R;
			cur = pre;
			continue;
		}
		if(brother->color == R){
			pre->left == cur ? lzhuan(&brother) : rzhuan(&brother);
			brother->color = B, pre->color = R;
			continue;
		}
		else{
			Node* near = NULL, *far = NULL;
			if(pre->left == cur) near = brother->left,  far = brother->right;
			else                 near = brother->right, far = brother->left;

			if(far == NULL || far->color == B){
				brother->left == far ? lzhuan(&near) : rzhuan(&near);
				far = brother, brother = near;
				brother->color = B, far->color = R;
			}
			pre->left == cur ? lzhuan(&brother) : rzhuan(&brother);
			brother->color = pre->color;
			pre->color = B;
			far->color = B;
			break;
		}
	}
}


/*
 * ����
 */
int main(){
	RB_tree a;
	for(int i = 0; i < 100; ++i){
		int t = rand() % 1000;
		a.insert(t);

	}
	deque<Node*> shit;
	shit.push_back(a.root);
	while(!shit.empty()){
		int size = shit.size();
		while(size-- > 0){
			Node* tmp = shit.front();
			shit.pop_front();
			if(tmp == NULL) {
				cout << "NULL" << ", ";
				continue;
			}
			shit.push_back(tmp->left);
			shit.push_back(tmp->right);
			cout << tmp->val << " " << tmp->color << ", ";
		}
		cout << endl;
	}
	a.print(); cout << endl;
	Node* q = a.find(190);
	cout << q->val << ", " << q->p->val << ", " << q->left << q->right << endl;
	for(int i = 0; i < 1000; ++i){
		int t = rand() % 1000;
		a.remove(t);
		if(is_BRtree(a.root) == 0) break;
		cout << "*******************" << is_BRtree(a.root) << endl;
	}
	//a.print();
	//cout << endl;
	cout << is_BRtree(a.root) << endl;
	a.print();
}
