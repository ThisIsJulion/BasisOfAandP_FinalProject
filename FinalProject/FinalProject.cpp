#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <iomanip> 
#include <vector> 
#include <cstdlib>

using namespace std;

struct Perfume{ //структура списка духов
	int id;
	string brand;
	string name;
	string type;
	double volume;
	double price;
};

struct Purchase { //структура, описывающ. одну позицию в корзине
	int purch_id;
	Perfume Piece;
	int quantity;
	double cost;
};

struct Delivery { //струк-ра, описывающая детали доставки
	string name;
	string address;
	string phone_number;
	enum Payment {card=1,cash=2,certificate=3};
	int payment_method;
};

void getDataFromFile(const char *path_to_file,Perfume *cosmetics); //фун-ция для заполнения массива типа структура путем считывания из файла
void showAllPerfume(Perfume cosmetics[10]); //фун-ция для вывода всех элементов массива стуртуры духов
void showPerfumeWithFilter(Perfume cosmetics[10], string condition); //фун-ция для фильтрации данных по списку 
void getDataFromPerfumeList(Purchase *pTemp, Perfume *pCocmetics); //фун-ция для дозаполнения данными одной позиции в корзины из массива структур духов
void showLastPurchase(vector<Purchase> *List); //фун-ция для вывода последнего элемента из вектора корзины
void showDelivery(Delivery *delivery); //функ-ция для вывода деталей доставки
void showBusket(vector<Purchase> *List); //функ-ция для вывода содержимого корзины
double calculateTotalCostInCheck(vector<Purchase> *List); //функ-ция для подсчета стоимости заказа
void editPurchase(vector<Purchase> *List); //функция для редактирования одной позиции в корзине
void editBusket(vector<Purchase>*List); //функция для редактирования корзины
void deletePurchase(vector<Purchase> *List); //функция для удаления одной позиции в корзине
void validateInput(); //функция для валидации вводимых данных
Delivery* createDelivery(); //функция для создания динамической структуры, кот. хранит в себе детали доставки 
Purchase* createPurchase(); //функция для создания динамической стуктуры, кот. хранит в себе детали одной позиции в корзине

int main()
{
	setlocale(LC_ALL, "Russian");
	string condition_main = "";
	const char *path_to_file = "D:\\Cosmetics\\Cosmetics.txt";
	int point = 0;
	int count_position = 0;

	Perfume *pCocmetics = new Perfume[10];
	getDataFromFile(path_to_file, pCocmetics);
	vector <Purchase> *pBusket = new vector<Purchase>; //создание динамич. вектора, который хранит все позиции в корзине

	cout << "Здравствуйте! \nВас приветствует онлайн-магазин парфюма." << endl;
	do {
		cout << "Меню для выбора: " << endl;
		cout << "1 - для просмотра всего списка парфюма;" << endl;
		cout << "2 - для просмотра только женского парфюма;" << endl;
		cout << "3 - для просмотра только мужского парфюма;" << endl;
		cout << "4 - для добавления товара в корзину;" << endl;
		cout << "5 - для просмотра содержимого корзины;" << endl;
		cout << "6 - для редактирования корзины;" << endl;
		cout << "7 - для оформления покупки;" << endl;
		cout << "8 - для выхода из онлайн-магазина;" << endl;
		cout << "\nПожалуйста, сделайте выбор: " << endl;
		cin >> point;
		if (!cin) {
			validateInput();
			cin >> point;
		}
		switch (point)
		{
		case 1:
			cout << "\n			***НАШ АССОРТИМЕНТ***\n" << endl;
			showAllPerfume(pCocmetics);
			break;
		case 2: 
			cout << "\n			***АССОРТИМЕНТ ЖЕНСКОГО ПАРФЮМА***\n" << endl;
			showPerfumeWithFilter(pCocmetics, "Women");
			break;
		case 3:
			cout << "\n			***АССОРТИМЕНТ МУЖСКОГО ПАРФЮМА***\n" << endl;
			showPerfumeWithFilter(pCocmetics, "Men");
			break;
		case 4:
		{
			cout << "\n			***ДОБАВЛЕНИЕ ТОВАРА В КОРЗИНУ***" << endl;
 			Purchase *purchase = createPurchase();
			getDataFromPerfumeList(purchase, pCocmetics);
			count_position++; 
			purchase->purch_id = count_position; //присваивание № позиции для каждой стурктуры в векторе
			pBusket->push_back(*purchase); //добавление товара в вектор (корзину)
			cout << "\nСледующий товар успешно добавлен в корзину: \n" << endl;
			showLastPurchase(pBusket);
			delete purchase;
			break; 
		}
		case 5:
		{
			if (pBusket->empty()) {
				cout << "\nВаша корзина пуста." << endl;
				break;
			}
			cout << "\n					***СОДЕРЖИМОЕ КОРЗИНЫ***\n" << endl;
			showBusket(pBusket);
			cout << "\nИтоговая сумма: " << calculateTotalCostInCheck(pBusket) << " BYN." << endl;
			break;
		}
		case 6: 
		{
			if (pBusket->empty()) {
				cout << "\nВаша корзина пуста. Вернитесь в главное меню и заполните ее." << endl;
				break;
			}
			cout << "\n					***СОДЕРЖИМОЕ КОРЗИНЫ***\n" << endl;
			showBusket(pBusket);
			cout << "\n					***РЕДАКТИРОВАНИЕ КОРЗИНЫ***\n" << endl;
			editBusket(pBusket);
			break;
		}
		case 7: {
			if (pBusket->empty()) {
				cout << "\nВаша корзина пуста. Вернитесь в главное меню и заполните ее." << endl;
				break;
			}
			cout << "\n					***СОДЕРЖИМОЕ ЗАКАЗА***\n" << endl;
			showBusket(pBusket);
			cout << "\nИтоговая сумма к оплате: " << calculateTotalCostInCheck(pBusket) << " BYN." << endl;
			cout << "\n					***ОФОРМЛЕНИЕ ДОСТАВКИ***\n" << endl;
			Delivery *delivery = createDelivery();
			showDelivery(delivery);
			delete delivery; //удаление указателя
			pBusket->clear(); //очистка вектора
			count_position = 0; //зануление позиции
			break;
		}
		case 8: {
			cout << "\nБудем ждать Вас снова! ";
			condition_main = "N";
			break;
		}
		default:
			cout << "\nДля корректного выбора введите от 1 до 8." << endl;
			break;
		}
		if (point != 8) {
			cout << "\nДля продолжения работы и возврата в основное меню введите Y" << endl;
			cout << "Для выхода из онлайн-магазина введите N" << endl;
			cout << " >> ";
			cin >> condition_main;
			cout << endl;
		}
	} while ((condition_main == "y") || (condition_main == "Y")); 

	cout << "До свидания!\nЗавершение работы программы." << endl;

	_getch();
}
void validateInput() {
	cout << "\nНекорректный ввод данных. Попробуйте еще раз: ";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void getDataFromFile(const char *path_to_file, Perfume *cosmetics) {
	ifstream file;
	file.open(path_to_file);
	if (!file.is_open()) {
		cout << "Ошибка чтения файла." << endl;
	}
	else {
		for (int i = 0; i < 10; i++) {
			file >> cosmetics[i].id;
			file >> cosmetics[i].brand;
			file >> cosmetics[i].name;
			file >> cosmetics[i].type;
			file >> cosmetics[i].volume;
			file >> cosmetics[i].price;
		}
		file.close();
	}
}
void showAllPerfume(Perfume cosmetics[10]) {
	cout << setw(5) << "КОД" << setw(16) << "БРЕНД" << setw(18) << "НАЗВАНИЕ" << setw(10) << "ТИП" <<
		setw(15) << "ОБЪЕМ(мл.)" << setw(15) << "ЦЕНА (BYN)" << endl;
	cout << "--------------------------------------------------------------------------------"<<endl;
	for (int i = 0; i < 10; i++) {
		cout << setw(5) << cosmetics[i].id;
		cout << setw(16) << cosmetics[i].brand;
		cout << setw(18) << cosmetics[i].name;
		cout << setw(10) << cosmetics[i].type;
		cout << setw(15) << cosmetics[i].volume;
		cout << setw(15) << cosmetics[i].price << endl;
	}
}
void showPerfumeWithFilter(Perfume cosmetics[10], string condition) {
	cout << setw(5) << "КОД" << setw(16) << "БРЕНД" << setw(18) << "НАЗВАНИЕ" << setw(10) << "ТИП" <<
		setw(15) << "ОБЪЕМ(мл.)" << setw(15) << "ЦЕНА (BYN)" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < 10; i++) {
		if (cosmetics[i].type == condition) {
			cout << setw(5) << cosmetics[i].id;
			cout << setw(16) << cosmetics[i].brand;
			cout << setw(18) << cosmetics[i].name;
			cout << setw(10) << cosmetics[i].type;
			cout << setw(15) << cosmetics[i].volume;
			cout << setw(15) << cosmetics[i].price << endl;
		}
	}
}
Purchase* createPurchase() {
	Purchase *pTemp = new Purchase();
	cout << "\nВведите код парфюма для добавления его в корзину: ";
	cin >> pTemp->Piece.id;
	while ((pTemp->Piece.id < 1) || (pTemp->Piece.id > 10)||(!cin)) {
		validateInput();
		cin >> pTemp->Piece.id;
	}
	cout << "\nВведите количество: ";
	cin >> pTemp->quantity;
	while ((!cin)||(pTemp->quantity <= 0)) {
		validateInput();
		cin >> pTemp->quantity;
	}
	return pTemp;
}
void getDataFromPerfumeList(Purchase *pTemp, Perfume *pCocmetics) {
	for (int i = 0; i < 10; i++) {
		if (pTemp->Piece.id == pCocmetics[i].id) {
			pTemp->Piece.brand = pCocmetics[i].brand;		
			pTemp->Piece.name = pCocmetics[i].name;
			pTemp->Piece.price = pCocmetics[i].price;
			pTemp->Piece.type = pCocmetics[i].type;
			pTemp->Piece.volume = pCocmetics[i].volume;
		}
	}
}
void showLastPurchase(vector<Purchase> *List) {
	cout << setw(5) << "КОД" << setw(16) << "БРЕНД" << setw(18) << "НАЗВАНИЕ" << setw(10) << "ТИП" <<
		setw(15) << "ОБЪЕМ(мл.)" << setw(15) << "ЦЕНА (BYN)" << setw(15)<< "КОЛ-ВО" << endl;
	cout << "----------------------------------------------------------------------------------------------" << endl;
	Purchase lastPurchase = List->back();
	cout << setw(5) << lastPurchase.Piece.id;
	cout << setw(16) << lastPurchase.Piece.brand;
	cout << setw(18) << lastPurchase.Piece.name;
	cout << setw(10) << lastPurchase.Piece.type;
	cout << setw(15) << lastPurchase.Piece.volume;
	cout << setw(15) << lastPurchase.Piece.price;
	cout << setw(15) << lastPurchase.quantity << endl;
}
void showBusket (vector<Purchase> *List) {
	cout << setw(7) << "ПОЗИЦИЯ" << setw(16) << "БРЕНД" << setw(18) << "НАЗВАНИЕ" << setw(10) << "ТИП" <<
		setw(15) << "ОБЪЕМ(мл.)" << setw(15) << "ЦЕНА (BYN)" << setw(15) << "КОЛ-ВО"  << setw(20) << "СТОИМОСТЬ (BYN)" << endl;
	cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < List->size(); i++) {
		Purchase purchase = List->at(i);
		cout << setw(7) << i+1;
		cout << setw(16) << purchase.Piece.brand;
		cout << setw(18) << purchase.Piece.name;
		cout << setw(10) << purchase.Piece.type;
		cout << setw(15) << purchase.Piece.volume;
		cout << setw(15) << purchase.Piece.price;
		cout << setw(15) << purchase.quantity;
		cout << setw(20) << purchase.quantity*purchase.Piece.price << endl;
	}
}
double calculateTotalCostInCheck(vector<Purchase> *List) {
	double totalCost = 0;
	for (int i = 0; i < List->size(); i++) {
		Purchase purchase = List->at(i);
		purchase.cost = purchase.Piece.price*purchase.quantity;
		List->at(i).cost = purchase.cost;
		totalCost = totalCost + List->at(i).cost;
	}
	return totalCost;
}
Delivery* createDelivery() {
	Delivery *pDelivery = new Delivery;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	cout << "\nВведите ваше имя латиницей: ";
	getline(cin, pDelivery->name);
	while (pDelivery->name.empty()) {
		cout << "\nЭто поле обязательно для заполнения. Попробуйте еще раз: ";
		getline(cin, pDelivery->name);
	}

	cout << "Введите адрес доставки латиницей: ";
	getline(cin, pDelivery->address);
	while (pDelivery->address.empty()) {
		cout << "\nЭто поле обязательно для заполнения. Попробуйте еще раз: ";
		getline(cin, pDelivery->address);
	}

	cout << "Введите номер телефона с кодом оператора для связи: ";
	getline(cin, pDelivery->phone_number);
	while (pDelivery->phone_number.empty()) {
		cout << "\nЭто поле обязательно для заполнения. Попробуйте еще раз: ";
		getline(cin, pDelivery->phone_number);
	}
	cout << "\nВыберите способ оплаты при получении товара: ";
	cout << "\n - Введите " << pDelivery->card << " при оплате картой;";
	cout << "\n - Введите " << pDelivery->cash << " при оплате наличными;";
	cout << "\n - Введите " << pDelivery->certificate << " если у Вас подарочный сертификат;" << endl;
	cout << " >> ";
	cin >> pDelivery->payment_method;
	while ((!cin)||(pDelivery->payment_method < 1)||(pDelivery->payment_method > 3))
	{
		validateInput();
		cin >> pDelivery->payment_method;
	}
	return pDelivery;
}
void showDelivery(Delivery *delivery) {
	
	cout << "\n------------------------------------------------" << endl;
	cout << "Ваш заказ №" << rand() << " успешно оформлен!" << endl;
	cout << "------------------------------------------------" << endl;
	cout << "\nДетали заказа: "<<endl;
	cout << "Имя покупателя - " << delivery->name << endl;
	cout << "Адрес доставки: " << delivery->address << endl;
	cout << "Контактный номер для связи: " << delivery->phone_number << endl;
}
void editPurchase(vector<Purchase> *List) {
	int position = 0;
	cout << "\nВведите позицию товара для редактирования: ";
	cin >> position;
	while ((!cin) || (position < List->front().purch_id) || (position > List->back().purch_id)) {
		validateInput();
		cin >> position;
	}
	for (int i = 0; i < List->size(); i++) {
		if (position == List->at(i).purch_id) {
			cout << "Введите желаемое количество: ";
			cin >> List->at(i).quantity;
			cout << "\nПозиция № " << List->at(i).purch_id << " была успешно отредактирована!" << endl;
		}
	}
}

void deletePurchase(vector<Purchase> *List) {
	int position = 0;
	cout << "\nВведите позицию товара для удаления: ";
	cin >> position;
	while ((!cin) || (position < List->front().purch_id) || (position > List->back().purch_id)) {
		validateInput();
		cin >> position;
	}
	for (int i = 0; i < List->size(); i++) {
		if (position == List->at(i).purch_id) {
			List->erase(List->begin()+i);
			cout << "\nПозиция № " << position << " была успешно удалена!" << endl;
		}
	}
}
void editBusket(vector<Purchase> *List) {
	int action = 0;
	cout << "Bыберите действие: " << endl;
	cout << "Введите 1 - для редактирования количества товара в корзине" << endl;
	cout << "Введите 2 - для удаления товара в корзине" << endl;
	cout << " >> ";
	cin >> action;
	while ((!cin) || ((action != 1) && (action != 2))) {
		validateInput();
		cin >> action;
	}
	if (action == 1) {
		editPurchase(List);
	}
	else {
		deletePurchase(List);
	}
}

