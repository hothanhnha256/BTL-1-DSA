#include "main.h"
// #include <memory>

// struct AllocationMetrics{
// 	uint64_t totalAllocated=0;
// 	uint64_t totalFreed=0;

// 	uint64_t currentUsage(){return totalAllocated-totalFreed;}
// };

// static AllocationMetrics s_AllocationMetrics;

// void* operator new(size_t size){
// 	s_AllocationMetrics.totalAllocated +=size;
// 	return malloc(size);
// }

// void operator delete(void *memory,size_t size){
// 	s_AllocationMetrics.totalFreed +=size;
// 	free(memory);
// }

// static void printMemoryUsage(){
// 	cout<<"Memory Usage: "<<s_AllocationMetrics.currentUsage()<<" bytes\n";
// }
class imp_res : public Restaurant
{
	public:
	class CustomerTime{
		public:
		customer* A;
		int Time;
		bool Name;
		CustomerTime*next;
		CustomerTime*prev;
		public:
		CustomerTime(){
			A=nullptr;
			Time=0;
			next=nullptr;
			prev=nullptr;
			Name=true;
		}
		CustomerTime(customer*A,int Time,bool Name,CustomerTime*next=nullptr,CustomerTime*prev=nullptr){
			this->Name=Name;
			this->A=A;
			this->Time=Time;
			this->next=next;
			this->prev=prev;
		}
		~CustomerTime(){
			A=nullptr;
			next=nullptr;
			prev=nullptr;
			delete A;
			delete next;
			delete prev;
		}
	};
	private:
	customer*X;

	CustomerTime*head;
	CustomerTime*tail;

	customer*waitingHead;
	customer*waitingTail;

	int CountALL;
	int CountTable;
	int CountWaiting;

	int Timing;
	public:	
		imp_res() {
			X=nullptr;
			head=nullptr;
			tail=nullptr;
			waitingHead=nullptr;
			waitingTail=nullptr;
			CountALL=0;
			CountTable=0;
			CountWaiting=0;
			Timing=0;
		};

        /*FOR RED*/
		void addTime(customer*cus,int time,bool Name){
			CustomerTime*temp=new CustomerTime(cus,time,Name);
			if(CountALL==0){
				head=temp;
				tail=temp;
			}
			else if(CountALL==1){
				tail=temp;
				head->next=tail;
				tail->prev=head;
			}
			else{
				tail->next=temp;
				temp->prev=tail;
				tail=temp;
			}
			CountALL++;
		}
		void addTable1(customer*cus){
			if(CountTable==0){
				cus->next=cus;
				cus->prev=cus;
			}
			else if(CountTable==1){
				X->next = cus;
				cus->prev = X;
				cus->next = X;
				X->prev = cus;
			}
			else{
				if (cus->energy >= X->energy) {
					customer* RIGHT = X->next;
					X->next = cus;
					cus->prev = X;
					cus->next = RIGHT;
					RIGHT->prev = cus;
				}
				else {
					customer* LEFT = X->prev;
					X->prev = cus;
					cus->prev = LEFT;
					cus->next = X;
					LEFT->next = cus;
				}
			}
			X=cus;
			CountTable++;
		}
		void addRES(customer*cus,bool SIGN){
			if(CountTable==0){
			}
			else if(CountTable==1){
				/*INSERT RIGHT*/
				X->next = cus;
				cus->prev = X;
				cus->next = X;
				X->prev = cus;
			
			}
			else {
				if (SIGN) {
					customer* RIGHT = X->next;
					X->next = cus;
					cus->prev = X;
					cus->next = RIGHT;
					RIGHT->prev = cus;
					
				}
				else {
					customer*LEFT = X->prev ;
					X->prev = cus;
					cus->prev = LEFT;
					cus->next = X;
					LEFT->next = cus;
					
				}
			}
			X=cus;
			CountTable++;
		}
		void addTable2(customer*cus){
			int k = cus->energy;
			customer* TEMP = X;
			bool SIGN=true;
			customer* MaxRES = X;
			int RES=0;
			for (int i = 0; i < CountTable; i++) {
				if ( TEMP&&RES <abs( k- TEMP->energy ) ) {
					MaxRES = TEMP;
					if (k - TEMP->energy > 0) {
						SIGN = true; 
					}
					else {
						SIGN = false; 
					}
					RES = abs(k - TEMP->energy);
				}
				TEMP = TEMP->next;
			}
			X=MaxRES;
			addRES(cus,SIGN);
		}
		void addWaiting(customer*cus){
			if(CountWaiting==0){
				waitingHead=cus;
				waitingTail=cus;
			}
			else if(CountWaiting==1){
				waitingTail=cus;
				waitingTail->prev=waitingHead;
				waitingHead->next=waitingTail;
			}
			else{
				waitingTail->next=cus;
				cus->prev=waitingTail;
				waitingTail=cus;
			}
			CountWaiting++;
		}
		bool checkName(string name){
			CustomerTime*travel=head;
			while(travel!=nullptr){
				if(travel->A->name==name)return true;
				travel=travel->next;
			}
			return false;
		}
		
	    /*FOR BLUE*/
		CustomerTime*FindfromWaiting(customer*waiting){
			CustomerTime*travel=head;
			while(travel!=nullptr){
				if(travel->A==waiting)return travel;
				travel=travel->next;
			}
		}
		void popTable(){
			CustomerTime*travel=head;
			while(travel!=nullptr){
				if(travel->Name)break;
				travel=travel->next;
			}
		    
			CustomerTime*RIGHT=travel->next;
			CustomerTime*LEFT=travel->prev;
			customer*temp=travel->A;
		
			if(CountTable==1){
				X=nullptr;
			}
			else{
				customer*left=temp->prev;
				customer*right=temp->next;

				if(temp->energy>=0){
					X=right;
				}
				else{
					X=left;
				}
				
				left->next=right;
				right->prev=left;
			}
			
			if(LEFT ==nullptr&&RIGHT==nullptr){
				head=nullptr;
				tail=nullptr;
			}
			else if(LEFT==nullptr){
				head=travel->next;
				head->prev=nullptr;
			}
			else if(RIGHT==nullptr){
				tail=travel->prev;
				tail->next=nullptr;
			}
			else{
				LEFT->next=RIGHT;
				RIGHT->prev=LEFT;
			}
			delete travel;
			delete temp;
		}
        void addFromWaiting(){
		    for(CountTable;CountTable<MAXSIZE&&CountWaiting>0;){
				//<<"STEP THU N\n";
				customer*temp=waitingHead;
				waitingHead=waitingHead->next;
				CustomerTime*temp2=FindfromWaiting(temp);
				temp2->Name=true;
				if(temp->next!=nullptr){temp->next->prev=nullptr;}
				if(CountTable<MAXSIZE/2){
					addTable1(temp);
				}
				else if(CountTable>=MAXSIZE/2){
					addTable2(temp);
				}
				CountWaiting--;
			}
		}
		
		/*FOR PURPLE*/
        
        void swapInWaitingver2(customer*A,customer*B){
			        customer* temp1 = A->next;
					customer* temp2 = A->prev;
					A->prev = B->prev;
					if(B->next == A){
						A->next = B;
					}
					else{
						A->next = B->next;
					}
					B->next = temp1;
					if(temp2 == B){
						B->prev = A;
					}
					else{
						B->prev = temp2;
					}
					if(A->prev != nullptr){
						A->prev->next = A;
					}
					A->next->prev = A;

					if (B->next != nullptr){
						B->next->prev = B;
					}

					B->prev->next = B;
					
					if (B == waitingHead)waitingHead = A;
					if (A ==waitingTail)waitingTail = B;

		}

		customer* FindPP(customer*current,int step){
			if(step>0){
			customer*temp=current;
			for(int i=0;i<step;i++){
				temp=temp->next;
				if(temp==nullptr)return nullptr;
			}
			return temp;
			}
			else{
				customer*temp=current;
		    	for(int i=0;i<-step;i++){
				temp=temp->prev;
				if(temp==nullptr)return nullptr;
			}
			return temp;
			}
		}
        int InSort( customer*First,int K ,  int n,int incr){
			int Counting=0;
			for(int i=incr;i<n;i+=incr){
				
				customer* Temp=FindPP(waitingHead,K+i);
				customer*TEMP=Temp;
				for(int j=i; (j>=incr) &&(
					(abs(TEMP->energy)>abs(FindPP(TEMP,-incr)->energy))
				   ||
				   ((abs(TEMP->energy)==abs(FindPP(TEMP,-incr)->energy))&&(FindfromWaiting(TEMP)->Time <FindfromWaiting(FindPP(TEMP,-incr))->Time   )) )
				    ;
					j-=incr,
					TEMP=FindPP(TEMP,-incr))
				    {
					swapInWaitingver2(TEMP,FindPP(TEMP,-incr));
					TEMP=FindPP(TEMP,incr);
					Counting++;
				}
			}
			return Counting;
		}
		
		int ShellShort(customer*First,customer*Last,int n){
			int Counting=0;
			for(int i=n/2;i>2;i/=2){
				for(int j=0;j<i;j++){
                    Counting+=InSort(First,j,n-j,i);
				}
			}
			Counting+=InSort(First,0,n,1);
			return Counting;
		}

        /*FOR REVERSAL*/
		void swapIncircle(customer* Left, customer* Right ){
			customer*LLeft=Left->prev;
			customer*RRight=Right->next;
			customer*RLeft=Left->next;
			customer*LRight=Right->prev;
			if(CountTable==2){
				return;
			}

			if(Left->next==Right){
				Left->next=RRight;
				RRight->prev=Left;
				Left->prev=Right;
				Right->next=Left;
				Right->prev=LLeft;
				LLeft->next=Right;
			}

			else if(Left->prev==Right){
				Left->prev=LRight;
				LRight->next=Left;
				Left->next=Right;
				Right->prev=Left;
				Right->next=RLeft;
				RLeft->prev=Right;
			}

			else{
				Left->next=RRight;
				RRight->prev=Left;
				Left->prev=LRight;
				LRight->next=Left;

				Right->next=RLeft;
				RLeft->prev=Right;
				Right->prev=LLeft;
				LLeft->next=Right;
			}
		}
		
		/*DONE RED*/
		void RED(string name, int energy)
		{
			
			if (energy == 0) { return; }
			if(!checkName(name)){
			customer *cus = new customer (name, energy, nullptr, nullptr);
			if(CountTable<MAXSIZE/2){
				addTable1(cus);
				addTime(cus,Timing,true);
			}
			else if(CountTable>=MAXSIZE/2&&CountTable<MAXSIZE){
				addTable2(cus);
				addTime(cus,Timing,true);
				
			}
			else if(CountWaiting<MAXSIZE){
				addWaiting(cus);
				addTime(cus,Timing,false);
			}
			else{
				delete cus;
			    }
			}
			Timing++;
		}
		/*DONE BLUE*/
		void BLUE(int num)
		{   if(CountALL==0)return;
			
			if(num>=CountTable){
				num=CountTable;
			}
			for(int i=0;i<num;i++){
				
		        customer*check=X;
			    CustomerTime*check2=head;
				popTable();
				CountALL--;
				CountTable--;
			}
			////<<"DONE\n";
			addFromWaiting();

		}
		
		/*SOME WRONG IN PURPLE*/
		void PURPLE()
		{
			if(CountWaiting<2){return;}
			int Max=0;
			int Time=0;
			customer*travel=waitingHead;
			customer*save=nullptr;
			int index=1;
			int TIME=0;
			while(travel!=nullptr){
					if(abs(travel->energy)>Max){
						Max=abs(travel->energy);
						save=travel;
					}
					else if(abs(travel->energy)==Max){
						if(FindfromWaiting(travel)->Time>FindfromWaiting(save)->Time){
							save=travel;
						}
					}
				travel=travel->next;
			}
			travel=waitingHead;
			while(travel!=save){
				index++;
				travel=travel->next;
			}
			
			BLUE(ShellShort(waitingHead,save,index)%MAXSIZE);
			

		}
		
		/*DONE REVERSAL*/
		void REVERSAL()
		{   
			int countOANLINH=0;
			int countCHUSU=0;
			customer*travel=X;
			for (int i=0;i<CountTable;i++){
				if(travel->energy>0){countCHUSU++;}
				else{
					countOANLINH++;
				}
				travel=travel->next;
			}
			countCHUSU/=2;
			countOANLINH/=2;
			customer*temp=X;
			if(CountTable<=2)return;
			customer*LEFT=X;
			customer*RIGHT=X->next;
			if(X->energy<0){
			while(countCHUSU>0){
				if(LEFT->energy>0&&RIGHT->energy>0){
					countCHUSU--;
					customer*temp1=LEFT;
					customer*temp2=RIGHT;
					LEFT=LEFT->prev;
					RIGHT=RIGHT->next;
					
					swapIncircle(temp1,temp2);
				}
				else if(LEFT->energy<0){
					LEFT=LEFT->prev;
				}
				else if(RIGHT->energy<0){
					RIGHT=RIGHT->next;
				}
			}
			LEFT=X;
			RIGHT=X->next;
			/*DUONG*/
			while(countOANLINH>0){
				if(LEFT->energy<0&&RIGHT->energy<0){
					countOANLINH--;
					customer*temp1=LEFT;
					customer*temp2=RIGHT;
					LEFT=LEFT->prev;
					RIGHT=RIGHT->next;
					
					swapIncircle(temp1,temp2);
				}
				else if(LEFT->energy>0){
					LEFT=LEFT->prev;
				}
				else if(RIGHT->energy>0){
					RIGHT=RIGHT->next;
				}
			}
			}
			else{
				while(countOANLINH>0){
				if(LEFT->energy<0&&RIGHT->energy<0){
					countOANLINH--;
					customer*temp1=LEFT;
					customer*temp2=RIGHT;
					LEFT=LEFT->prev;
					RIGHT=RIGHT->next;
					
					swapIncircle(temp1,temp2);
				}
				else if(LEFT->energy>0){
					LEFT=LEFT->prev;
				}
				else if(RIGHT->energy>0){
					RIGHT=RIGHT->next;
				}
			}
			LEFT=X;
			RIGHT=X->next;
			/*DUONG*/
			while(countCHUSU>0){
				if(LEFT->energy>0&&RIGHT->energy>0){
					countCHUSU--;
					customer*temp1=LEFT;
					customer*temp2=RIGHT;
					LEFT=LEFT->prev;
					RIGHT=RIGHT->next;
					
					swapIncircle(temp1,temp2);
				}
				else if(LEFT->energy<0){
					LEFT=LEFT->prev;
				}
				else if(RIGHT->energy<0){
					RIGHT=RIGHT->next;
				}
			}
			}
			
		}
		
		/*FOR UNLIMITED*/
		void PRINTNAMEVER2(customer*LEFT,customer*RIGHT){
			int MinVALUE=LEFT->energy;
			customer*MINNODE=LEFT;
			customer*travel=LEFT;
			while(travel!=RIGHT){
				if(MinVALUE>travel->energy){
					MinVALUE=travel->energy;
					MINNODE=travel;
				}
				travel=travel->next;
			}
			if(MinVALUE>travel->energy){
					MinVALUE=travel->energy;
					MINNODE=travel;
			}
			customer*temp=MINNODE;
			while(MINNODE!=RIGHT){
				MINNODE->print();
				MINNODE=MINNODE->next;
			}
			MINNODE->print();
			MINNODE=LEFT;
			while(MINNODE!=temp){
				MINNODE->print();
				MINNODE=MINNODE->next;
			}
		}
		void UNLIMITED_VOID()
		{
			if(CountTable<4)return;

			customer*leftSave=X;
			customer*rightSave=X;
			customer*LEFT=X;
			int MinVALUE=INT16_MAX;
			int countMax=0;
			for(int j=0;j<CountTable;j++){
				customer*temp2=LEFT;
				int sum=0;
				for(int i=1;i<=CountTable;i++){
					
					sum+=temp2->energy;
					if(i<4){
						temp2=temp2->next;
						continue;
					}
				    if(MinVALUE>sum||(MinVALUE==sum&&countMax<=i)){
					    MinVALUE=sum;
					    leftSave=LEFT;
					    rightSave=temp2;
						countMax=i;
				    }
					temp2=temp2->next;
				}
				LEFT=LEFT->next;
			}
			PRINTNAMEVER2(leftSave,rightSave);
		}

		/*FOR DOMAIN*/
		void deleteCustomerTime(CustomerTime*cus){
			if(CountALL==1){
				head=nullptr;
				tail=nullptr;
				cus->next=nullptr;
				cus->prev=nullptr;
			
				delete cus;
			}
			else if(CountALL==2){
				if(cus==head){
					head=head->next;
					tail->prev=nullptr;
					head->prev=nullptr;
				}
				else{
					tail=tail->prev;
					head->next=nullptr;
					tail->next=nullptr;
				}
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else{
			    if(cus==head){
					head=head->next;
					head->prev=nullptr;
				}
				else if(cus==tail){
					tail=tail->prev;
					tail->next=nullptr;
				}
				else{
					CustomerTime*left=cus->prev;
					CustomerTime*right=cus->next;
					left->next=right;
					right->prev=left;
				}
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
		}
		void deleteCustomerWaiting(customer*cus){
			if(CountWaiting<3){
				if(CountWaiting==1){
					waitingHead=nullptr;
					waitingTail=nullptr;
				}
				if(CountWaiting==2){
					if(cus->next){
						waitingHead=cus->next;
						waitingTail=waitingHead;
					}
					else{
						waitingHead=cus->prev;
					    waitingTail=cus->prev;
					}
				}
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else{
			if(cus==waitingHead){
				waitingHead=waitingHead->next;
				waitingHead->prev=nullptr;
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else if(cus==waitingTail){
				waitingTail=waitingTail->prev;
				waitingTail->next=nullptr;
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else{
				customer*left=cus->prev;
				customer*right=cus->next;
				left->next=right;
				right->prev=left;
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			}
		}
		void deleteCustomerTable(customer* cus){
			if(CountTable==1){
				X=nullptr;
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else if(CountTable==2){
				X=cus->next;
				X->prev=X;
				X->next=X;
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
			else{
				customer*left=cus->prev;
				customer*right=cus->next;
				left->next=right;
				right->prev=left;
				if(cus->energy>0){
					X=right;
				}
				else{
					X=left;
				}
				cus->next=nullptr;
				cus->prev=nullptr;
				delete cus;
			}
		}
		void Fire(bool Sign){
            CustomerTime*travel1=tail;
            if(Sign){
                while(travel1!=nullptr){
                    if(travel1->A->energy>0){travel1->A->print();}
                    travel1=travel1->prev;
                }
            }
            else{
                while(travel1!=nullptr){
                    if(travel1->A->energy<0){travel1->A->print();}
                    travel1=travel1->prev;
                }
            }

			CustomerTime*travel=head;
			/*DUOI CHU SU*/
			if(Sign){
				while(travel!=nullptr){
					if(travel->A->energy>0){
						CustomerTime*temp=travel;
						travel=travel->next;

						customer*temp2=temp->A;
						if(temp->Name){
							deleteCustomerTable(temp2);
							CountTable--;
						}
						else{
							deleteCustomerWaiting(temp2);
							CountWaiting--;
						}
						deleteCustomerTime(temp);
						CountALL--;
					}
					else{
					travel=travel->next;
					}
				}
			}
			/*DUOI OAN LINH*/
			else{
				while(travel!=nullptr){
					if(travel->A->energy<0){
						CustomerTime*temp=travel;
						travel=travel->next;
						customer*temp2=temp->A;
						if(temp->Name){
							deleteCustomerTable(temp2);
							CountTable--;
						}
						else{
							deleteCustomerWaiting(temp2);
							CountWaiting--;
						}
						deleteCustomerTime(temp);
						CountALL--;
					}
					else{
					travel=travel->next;
					}
				}
			}
		}
		
		void DOMAIN_EXPANSION()
		{
			CustomerTime*travel=head;
			if(CountALL==0){return;}
			/*TIM COUNT*/
			int ALL=0;
			for(int i=0;i<CountALL;i++){
				ALL+=travel->A->energy;
				travel=travel->next;
			}
			Fire(ALL<0);
			addFromWaiting();
		}
		
		void LIGHT(int num)
		{

			if(CountALL==0)return;
			if(num>0){
				customer*travel=X;
				for(int i=0;i<CountTable;i++){
					travel->print();
					travel=travel->next;
				}
			}
			else if(num<0){
				customer*travel=X;
				for(int i=0;i<CountTable;i++){
					travel->print();
					travel=travel->prev;
				}
			} 
			else{
				customer*travel=waitingHead;
				for(int i=0;i<CountWaiting;i++){
					travel->print();
					travel=travel->next;
				}
			}
		}
		
	~imp_res(){
		
		while(head!=nullptr){
			
		    CustomerTime*intime=head;
			head=head->next;
			intime->next=nullptr;
			intime->prev=nullptr;
			delete intime->A;
			delete intime;
		}
		CountALL=0;
		CountTable=0;
		CountWaiting=0;
	}
		
};