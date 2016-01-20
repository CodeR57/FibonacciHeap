#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

struct node {
	int key;
	int degree;
	struct node* p;
	struct node* left;
	struct node* right;
	struct node* child;
	bool mark;
};

struct heap {
	struct node* min;
	int n;
};

struct heap* Make_Empty_Fib_Heap();
struct node* Make_New_Fib_Node(int k);
void Fib_Heap_Insert (struct heap* H, struct node* x);
struct node* Fib_Heap_Minimum (struct heap* H);
struct heap* Fib_Heap_Union(struct heap* H1, struct heap* H2);
struct node* Fib_Heap_Extract_Min (struct heap* H);
struct node* clean_p (struct node* x);
void Consolidate (struct heap* H);
void Fib_Heap_Link (struct heap* H, struct node* y, struct node* x);
void Fib_Heap_Decrease_Key (struct heap* H, struct node* x, int k);
void Cut (struct heap* H, struct node* x, struct node* y);
void Cascading_Cut (struct heap* H, struct node* y);
void Fib_Heap_Delete (struct heap* H, struct node* x);
void Print_Root_List (struct heap* H);

int main(){
	FILE* input;
	FILE* output;
	input = fopen("input.txt", "r");
	output = fopen("output.txt", "w");
	int i, n, k;
	struct node* x = NULL;
	struct heap* H1 = Make_Empty_Fib_Heap();
	struct heap* H2 = Make_Empty_Fib_Heap();
	struct heap* H = Make_Empty_Fib_Heap();
	while(1){
		if (feof(input)) break;
		fscanf(input, "%d", &n); //÷èñëî ýëåìåíòîâ â ïåðâîé êó÷å
		for (i=0; i<n; i++){
			fscanf(input, "%d", &k); //ñ÷èòûâàåì êëþ÷è ýòèõ ýëåìåíòîâ
			x = Make_New_Fib_Node(k);
			Fib_Heap_Insert (H1, x);
		}
		if (H1->min){
		struct node* x_right = H1->min->right;
		struct node* tmp = NULL;
		fprintf(output, "Root list:\n"); //Âûâîäèì íà ïå÷àòü ñïèñîê êîðíåé ïåðâîé êó÷è
		fprintf(output, "%d ", (Fib_Heap_Minimum(H1))->key); //Ïåðâûé ýëåìåíò - ìèíèìàëüíûé
		while (x_right != H1->min){
			  fprintf(output, "%d ", x_right->key);
			  tmp = x_right->right;
			  x_right = tmp;
		}
		fprintf(output, "\n");
		} else {
			fprintf(output, "No elements\n");
		}
		Print_Root_List(H1); // ýòî òî æå ñàìîå, íî âûâîäèò â êîíñîëü - îòëàäî÷íûé âûâîä
		fscanf(input, "%d", &n); //÷èñëî ýëåìåíòîâ âî âòîðîé êó÷å
		for (i=0; i<n; i++){
			fscanf(input, "%d", &k); //ñ÷èòûâàåì êëþ÷è ýòèõ ýëåìåíòîâ
			x = Make_New_Fib_Node(k);
			Fib_Heap_Insert (H2, x);
		}
		if (H2->min){ //Âûâîäèì íà ïå÷àòü ñïèñîê êîðíåé âòîðîé êó÷è
		struct node* x_right = H2->min->right;
		struct node* tmp = NULL;
		fprintf(output, "Root list:\n"); 
		fprintf(output, "%d ", (Fib_Heap_Minimum(H2))->key); //Ïåðâûé ýëåìåíò - ìèíèìàëüíûé
		while (x_right != H2->min){
			  fprintf(output, "%d ", x_right->key);
			  tmp = x_right->right;
			  x_right = tmp;
		}
		fprintf(output, "\n");
		} else {
			fprintf(output, "No elements\n");
		}
		Print_Root_List(H2); // ýòî òî æå ñàìîå, íî âûâîäèò â êîíñîëü - îòëàäî÷íûé âûâîä
		char s[5];
		fscanf(input, "%s", &s);
		if ((s[0]) == 'U'){
			H = Fib_Heap_Union(H1, H2);
			if (H->min){ //Âûâîäèì íà ïå÷àòü ñïèñîê êîðíåé îáúåäèí¸ííîé êó÷è
			   struct node* x_right = H2->min->right;
			   struct node* tmp = NULL;
			   fprintf(output, "Root list:\n"); 
			   fprintf(output, "%d ", (Fib_Heap_Minimum(H))->key); //Ïåðâûé ýëåìåíò - ìèíèìàëüíûé
			   while (x_right != H2->min){
			   		 fprintf(output, "%d ", x_right->key);
			  		 tmp = x_right->right;
			  		 x_right = tmp;
				}
				fprintf(output, "\n");
			} else {
			  	   fprintf(output, "No elements\n");
			}
			Print_Root_List(H); // ýòî òî æå ñàìîå, íî âûâîäèò â êîíñîëü - îòëàäî÷íûé âûâîä
		}
		if ((s[0]) == 'E'){
		   if(H->min == NULL){
				  fprintf(output, "\nError: Cannot extract anything\nThe Fib heap is EMPTY\n");
		} else {
		  	x = Fib_Heap_Extract_Min(H);
			fprintf(output, "%d is extracted\n", x->key);
			if (H->min) {
				fprintf(output, "Minimum is %d\n\n", H->min->key);
			 	}
			 }
		}
	}
	printf("\nSuccess!\nResults are in the file output.txt\n");
	fclose(input);
	fclose(output);
	free(H);
	return 0;
}

struct heap* Make_Empty_Fib_Heap(){ //O(1)
	struct heap* H = (struct heap* )malloc(sizeof(struct heap));
	H->min = NULL;
	H->n = 0;
	return H;
}

struct node* Make_New_Fib_Node (int k){ //O(1)
	struct node* x = (struct node* )malloc(sizeof(struct node));
	x->key = k;
	x->degree = 0;
	x->p = NULL;
	x->child = NULL;
	x->mark = false;
	return x;
}

void Fib_Heap_Insert (struct heap* H, struct node* x){ //O(1)
	if(H->min){
	 	H->min->right->left = x; 
	 	x->left = H->min;
	 	x->right = H->min->right;
		H->min->right = x;
		if (x->key < H->min->key){
			H->min = x;
		}
	 } else {
	 	x->left = x;
		x->right = x;
	 	H->min = x;
	 }
	 H->n++;
}

struct node* Fib_Heap_Minimum (struct heap* H){ //O(1)
	return H->min;;
}

struct heap* Fib_Heap_Union (struct heap* H1, struct heap* H2){ //O(1)
	//printf("Enter Union\n");
	struct heap* H = Make_Empty_Fib_Heap();
	if (H1->min&&H2->min){
		//printf("Enter Union 3\n");
		H->min = H1->min;
		H->min->left->right = H2->min->right; //4 ñòðîêè - îáúåäèíåíèå ñïèñêîâ
		H2->min->right->left = H->min->left;
		H->min->left = H2->min;
		H2->min->right = H->min;
		if ((H1->min == NULL) || ((H2->min) && (H2->min->key <= H1->min->key))) {
		   H->min = H2->min;
		   }
	} else if (H1->min) {
		H->min = H1->min;
	} else if (H2->min) {
		H->min = H2->min;
	}
	H->n = H1->n + H2->n;
	//printf("Leave Union\n");
	return H;
}

struct node* Fib_Heap_Extract_Min (struct heap* H){
	struct node* z = H->min;
	printf("I'm here 1\n");
	if (z) {
		printf("I'm here 2\n");
		if (z->child){
			printf("I'm here 3\n");
			struct node* x = clean_p(z->child);
			z->left->right = x; //ïîäíÿòèå äåòåé â ñïèñîê êîðíåé è óäàëåíèå ðîäèòåëÿ
			z->right->left = x->left;
			x->left->right = z->right;
			x->left = z->left;				
		}
		printf("I'm here 4\n");
		z->left->right = z->right; //óêàçàòåëè ïðè z íå ìåíÿþòñÿ
		z->right->left = z->left;
		printf("I'm here 5\n");
		if (z == z->right) { //z-åäèíñòâåííûé óçåë, ïîòîìêîâ íåò
			printf("I'm here 6\n");
			H->min = NULL;
		} else {
			printf("I'm here 7\n");
			H->min = z->right;
			printf("I'm here 8\n");
			Consolidate(H);
		}
		printf("I'm here 5\n");
		H->n--;		
		}
	return z;
}

struct node* clean_p (struct node* x){ //Ñëîæíîñòü O(log n)
	struct node* prev_x = x->left;
	struct node* tmp = NULL;
	x->p = NULL;
	while(tmp != prev_x){
	tmp = x->right;
	x = tmp;
	x->p = NULL;
	}
	return x;
}

void Consolidate (struct heap* H){
	printf("Enter consolidate\n");
	int Dn = (int)floor(H->n);
	int i;
	printf("consolidate 1\n");
	struct node** A = (struct node**)malloc(Dn*sizeof(struct node*));
	printf("consolidate 2\n");
	for (i=0; i<=Dn; i++){
		A[i] = NULL;
	}
	printf("consolidate 3\n");
	struct node* w = H->min;
	struct node* tmp = NULL;
	while (tmp != H->min) {
		printf("consolidate 4\n");
		struct node* x = w;
		int d = x->degree;
		while (A[d]){
			printf("consolidate 5\n");
			struct node* y = A[d];
			if (x->key > y->key){
				struct node* buf = x;
				x = y;
				y = buf;
			}
			Fib_Heap_Link(H, y, x);
			A[d] = NULL;
			d++;
		}
		A[d] = x;
		tmp = w->right;
		w = tmp;
	}
	printf("consolidate 6\n");
	H->min = NULL;
	for (i=0; i<=Dn; i++){
		if (A[i]){
			if (H->min == NULL){
				H->min = A[i];
			} else{
				H->min->left->right = A[i];
				A[i]->left = H->min->left;
				A[i]->right = H->min;
				H->min->left = A[i];
				if (A[i]->key < H->min->key){
					H->min = A[i];
				}
			}
		}
	}
}

void Fib_Heap_Link(struct heap* H, struct node* y, struct node* x){
	y->left->right = y->right;
	y->right->left = y->left;
	y->p = x;
	if (x->child){
		x->child->right->left = y;
		y->right = x->child->right;
		y->left = x->child;
		x->child->right = y;
	}
	x->child = y;
	x->degree++;
	y->mark = false;
}

void Fib_Heap_Decrease_Key (struct heap* H, struct node* x, int k){
	if (k > x->key) {
		printf ("Error! New key is bigger than current\n");
	} else {
		x->key = k;
		struct node* y = x->p;
		if (y && (x->key < y->key)){
			Cut(H, x, y);
			Cascading_Cut(H,y);
		}
		if(x->key < H->min->key){
			H->min = x;
		}
	}	
}

void Cut (struct heap* H, struct node* x, struct node* y){
	if (x == x->right){
		y->child = NULL; ////óäàëåíèå x èç ñïèñêà äî÷åðíèõ óçëîâ y
	} else {
		x->left->right = x->right; //óäàëåíèå x èç ñïèñêà äî÷åðíèõ óçëîâ y
		x->right->left = x->left;
	}
	y->degree--; //óìåíüøåíèå ñòåïåíè (êîëè÷åñòâà äî÷åðíèõ óçëîâ) y
	H->min->right->left = x; //äîáàâëåíèå x â ñïèñîê êîðíåé H
	x->right = H->min->right;
	x->left = H->min;
	H->min->right = x;
	x->p = NULL;
	x->mark = false;
}

void Cascading_Cut (struct heap* H, struct node* y){
	struct node* z = y->p;
	if (z){
		if (y->mark == false){
			y->mark = true;
		} else {
			Cut (H, y, z);
			Cascading_Cut(H,z);
		}
	}
}

void Fib_Heap_Delete (struct heap* H, struct node* x){
	Fib_Heap_Decrease_Key(H, x, -2147483647);
	struct node* y = Fib_Heap_Extract_Min(H);
}

void Print_Root_List (struct heap* H){ //O(log n) èç-çà öèêëà while
	if (H->min){
		struct node* x_right = H->min->right;
		struct node* tmp = NULL;
		printf("Root list:\n");
		printf("%d ", x_right->left->key);
		while (x_right != H->min){
			  printf("%d ", x_right->key);
			  tmp = x_right->right;
			  x_right = tmp;
		}
		printf("\n");
	} else {
		printf("No elements\n");
	}
}
