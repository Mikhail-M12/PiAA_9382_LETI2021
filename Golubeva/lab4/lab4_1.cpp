#include <iostream>
#include <vector>

void print_vec(std::vector<int >vec){
    for (int i = 0;i < vec.size(); i++){
        if (i == 0){
            std::cout << vec[i];
        }
        else{
            std::cout << ',' << vec[i];
        }    
    }
    std::cout<<"\n";
}

std::vector<int> KnutMorrisPratt(std::string text, std::string templ)
{
    std::vector<int> prefix(templ.size() + 1, -1);
    std::vector<int> matches;

    if (templ.size() == 0){
        matches.push_back(0);
        return matches;
    }

    //compute prefix function
    for (int i = 1; i <= templ.size(); i++){
        int position = prefix[i - 1];
        while (position != -1 && templ[position] != templ[i - 1]){
            position = prefix[position];
        }    
        prefix[i] = position + 1;
    }
    std::cout<< "Prefix function of template(without the -1 in beginning): ";
    print_vec(prefix);
    std::cout << '\n';
    
    int textpos = 0, templpos = 0;
    while (textpos < text.size()){
        while (templpos != -1 && (templpos == templ.size() || templ[templpos] != text[textpos])){
            templpos = prefix[templpos];
        } 
        std::cout << "templpos: " << templpos << ", textpos: " << textpos<<'\n';
        textpos++;
        templpos++;
        if (templpos == templ.size()){ // if found a match
            std::cout<<"find match at pos: " << textpos - templ.size() << '\n';
            matches.push_back(textpos - templ.size());
        }    
    }
    return matches;
}

int main(){
    std::string s1;
    std::string s2;
    std::cin >> s1;
    std::cin >> s2;
    std::vector <int> match = KnutMorrisPratt(s2, s1);
    if (match.size() == 0){
        std::cout<< "Result: " << -1 << '\n';
    }
    else{
        print_vec(match);
    }
}