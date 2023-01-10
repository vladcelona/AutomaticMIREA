#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdio>
#include <regex>
#include <Windows.h>
#include <fstream>
#include <fcntl.h>
#include <io.h>
#include <locale.h>
#include <aspose_ocr.h>
#include <pugixml.hpp>

#pragma execution_character_set("utf-8")
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#define all(x) begin(x), end(x)
#define end_line '\n'
#define space ' '
#define tab '\t'

typedef unsigned long long int ull;
typedef long long int ll;
typedef long double ld;

using namespace std;
using namespace pugi;


int input_language() {
	wcout << L"Вы хотите ввести слово в каком языке?" << end_line;
	wcout << L"0 - Английский (English)" << end_line;
	wcout << L"1 - Русский" << end_line;
	wcout << L"2 - Немецкий (Deutsch)" << end_line;
	wcout << L"3 - Испанский (Español)" << end_line;
	wcout << L"4 - Португальский (Português)" << end_line;
	wcout << L"5 - Китайский (中文)" << end_line;
	wcout << L"6 - Французский (Français)" << end_line;
	wcout << L"7 - Несколько языков." << end_line;

	wcout << L"-> ";

	int choice; cin >> choice; return choice;
}


void extract_text_from_image(const int& language_index) {

	// const string license_string = "license_file_path";
	// aspose::ocr::set_license(license_string.c_str());

	// Здесь указывается путь к файлу

	wcout << L"Введите название файла: ";
	string file_name; cin >> file_name;
	string image_path = "C:\\Screenshots\\" + file_name;

	// Здесь прописаны коды языков, которые могу быть выбраны пользователем
	vector<language> languages = {
		language::eng, language::rus, language::deu, language::spa,
		language::por, language::chi, language::fra, language::none
	};

	// Здесь прописаны элементы, необходимые для чтения картинки
	RecognitionSettings recognition_settings;
	recognition_settings.format = export_format::xml;
	recognition_settings.language_alphabet = languages[language_index];
	recognition_settings.save_format = file_format::txt;

	try {
		wcout << L"Обработка картинки" << end_line;

		aspose::ocr::page_save(
			image_path.c_str(),
			"C:\\Screenshots\\extracted_text_file.xml",
			recognition_settings
		);
	}
	catch (...) {
		wcout << L"Вы указали неправильный путь к файлу" << end_line << end_line;
		extract_text_from_image(language_index);
	}
}


void open_test_file() {
	xml_document xml_doc; xml_doc.load_file("C:\\Screenshots\\hello.xml");

	auto rotary = xml_doc.root();
	// rotary.print(std::cout);

	auto name = rotary.select_node("//UserInformation/Another/Name/text()").node();
	auto age = rotary.select_node("//UserInformation/Age/text()").node();

	wcout << L"\nName is " << name.value() << end_line;
	wcout << L"Age is " << age.text().as_double() << end_line;
}


string replace_string(string subject, const string& search, const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


inline bool file_exists(const string& filename) {
	ifstream check_file(filename); return check_file.good();
}


void open_result_file() {

	string extracted_text_file = "C:\\Screenshots\\extracted_text_file.xml";
	string output_result_text_file = "C:\\Screenshots\\output_result_text.txt";
	string result_text_file = "C:\\Screenshots\\result_text.txt";

	xml_document xml_doc; xml_doc.load_file(extracted_text_file.c_str());
	auto total_result = xml_doc.root();

	auto result_string = total_result.select_node("//page/text/text()").node();
	wcout << end_line << L"The result is: " << result_string.value() << end_line;
	// string result_string = xml_doc.child("totalResult").child("page").child("text").value();
	// wcout << L"The result: " << result_string.c_str() << end_line;

	ofstream output_file;
	output_file.open(output_result_text_file.c_str());
	output_file << result_string.value() << end_line; output_file.close();

	ifstream input_file;
	if (file_exists(output_result_text_file.c_str())) {
		input_file.open(output_result_text_file.c_str());
	}
	string text_from_file; getline(input_file, text_from_file);

	// wcout << text_from_file.c_str() << end_line;
	// text_from_file.erase(remove(all(text_from_file), '\\'), text_from_file.cend()); 
	input_file.close();

	// regex target("(\\n)+"); string replacement = "";
	// text_from_file = regex_replace(text_from_file, target, replacement);
	// text_from_file.erase(text_from_file.find("\\"), text_from_file.length() - 1);

	text_from_file = replace_string(text_from_file, "\\n", "");

	ofstream result_file(result_text_file.c_str());
	result_file << text_from_file.c_str() << end_line;
	result_file << "Длина слова: " << text_from_file.length() << end_line; result_file.close();

	wcout << text_from_file.c_str() << end_line;
	wcout << L"Длина слова: " << text_from_file.length() << end_line;

	ShellExecute(0, 0, L"C:\\Screenshots\\result_text.txt", 0, 0, SW_SHOW);
}

int wmain() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	SetConsoleCP(1251); SetConsoleOutputCP(1251);
	_setmode(_fileno(stdout), _O_U16TEXT);

	int chosen_language = input_language();
	wcout << L"Выбранный номер языка: " << chosen_language << end_line;

	extract_text_from_image(chosen_language);
	// open_test_file(); // Тестовый файл
	open_result_file(); // Основной файл 

	return EXIT_SUCCESS;
}