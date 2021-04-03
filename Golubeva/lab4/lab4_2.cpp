#include <iostream>
#include <vector>
#include <string>

void print_vec(std::vector<int >vec){
    for (int i = 0;i < vec.size(); i++){
        if (i == 0){
            std::cout << vec[i];
        }
        else{
            std::cout << ", " << vec[i];
        }    
    }
    std::cout<<"\n";
}

std::vector <int> prefixFunc(std::string string){
    int len_s = string.length();
    std::vector <int> prefix;
	prefix.resize(len_s);
    prefix[0] = 0;
    int ind = 0;	
	for (int i = 1; i < string.length(); i++){	
		while (ind > 0 && string[ind] != string[i]){
			ind = prefix[ind - 1]; 
        }    
		if (string[ind] == string[i]){
			ind += 1;
        }    
		prefix[i] = ind;
	}
	return prefix;
}

int isCyclicShift(std::string str1, std::string str2){
	int result = -1;
	if (str1.length() != str2.length()){ //unless exactly cyclic shift
		std::cout << "string lengths not matched! ";
		return result;
    }    

	std::string pasting_string = str1 + str1;
	std::vector <int> pref = prefixFunc(str2); // find prefix - function
	std::cout<< "Prefix function of str2: ";
    print_vec(pref);
    std::cout << '\n';
	int ind = 0;
	for (int i = 0; i < pasting_string.size(); i++){
		//find index in str2 to match characters in str1
		while (ind > 0 && str2[ind] != pasting_string[i]){
			ind = pref[ind - 1];
        }   
		std::cout<<"compare characters in position " << i << " in str1 and "<< ind <<" in str2"<<'\n'; 
		if (str2[ind] == pasting_string[i]){// check matches
        	ind += 1;
        }    
    
		if (ind == str2.length()) { // if find a substring in rasting string
			std::cout<<"find a match with position " << i - str2.length() + 1<< ", finish!";
			result = i - str2.length() + 1;
			break;
		}
	}
	return result;
}

int main() {
	std::string s1;
    std::string s2;
    std::cin >> s1;
    std::cin >> s2;
	int res = isCyclicShift(s1, s2);
	std::cout << "\nResult: " << res << '\n';
    return 0;
}