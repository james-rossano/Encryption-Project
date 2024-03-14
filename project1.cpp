// CSCI 1300 Spring 2024
// Author: James Rossano
// TA: Aditya Chandra

#include<iostream>
#include <iomanip>
#include <string>

using namespace std;

char shiftChar(char letter, int shift_value){
    int asciiVal = (int)letter;
    shift_value = shift_value % 26;

    if(isupper(letter)){
        if(asciiVal+shift_value >= 65 && asciiVal+shift_value <= 90){
            return char(asciiVal+shift_value);
        }else if(asciiVal+shift_value < 65){
            asciiVal = 91 - (65 - (asciiVal+shift_value));
            return char(asciiVal);
        }else if(asciiVal+shift_value > 90){
            asciiVal = 64 + ((asciiVal+shift_value) - 90);
            return char(asciiVal);
        }
        return letter%26;
    }else if(islower(letter)){
        if(asciiVal+shift_value >= 97 && asciiVal+shift_value <= 122){
            return char(asciiVal+shift_value);
        }else if(asciiVal+shift_value < 97){
            asciiVal = 123 - (97 - (asciiVal+shift_value));
            return char(asciiVal);
        }else if(asciiVal+shift_value > 122){
            asciiVal = 96 + ((asciiVal+shift_value) - 122);
            return char(asciiVal);
        }
        return letter%26;
    }

    return letter;
}

int getDoubleKeyShiftValue(string key1, string key2){
    int length1 = key1.length();
    int length2 = key2.length();
    int shift = 0;

    for(int g = 0; g < length1; g++){
        for(int i = 0; i < length2; i++){
            if(key2[i] == key1[g]){
                shift++;
            }
        }
    }

    return shift%26;
}

string getLayer1MessageEncryption(string message, int method_choice, string key1, string key2){
    string encrypt = "";
    int shift;
    int length = message.length();

    if(method_choice == 2){
        shift = getDoubleKeyShiftValue(key1, key2);
        
        for(int i = 0; i < length; i++){
            encrypt += shiftChar(message[i], shift);
        }
        return encrypt;
        
    } else if(method_choice == 1){
        shift = 13;
        
        for(int i = 0; i < length; i++){
            encrypt += shiftChar(message[i], shift);
        }
        return encrypt;
    }

    return "--Error--"; 
}

string getLayer2MessageEncryption(string message, string key_phrase){
    int Klength = key_phrase.length();
    int Mlength = message.length();
    string encrypt = "";
    int cryptL = 0;

    if(key_phrase == ""){
        return "--Error--";
    } else if(message == ""){
        return "--Error--";
    }
    
    for(int i = 0; i < Klength; i++){
        if(((int)toupper(key_phrase[i]) < 65 || (int)toupper(key_phrase[i]) > 90) && key_phrase[i] != ' '){
            return "--Error--";
        } else if(i == 0){
            encrypt += toupper(key_phrase[0]);
        }else if(key_phrase[i] == ' ' && i != Klength-1){
            encrypt += toupper(key_phrase[i+1]);
        }

        cryptL = encrypt.length();
        if(cryptL >= Mlength){
            break;
        }
    }

    int lengthE = encrypt.length();
    int charShift;
    int numSkips = 0;
    string Emessage = "";

    for(int i = 0; i < Mlength; i++){
        charShift = (int)encrypt[((i-numSkips) % (lengthE))] - 65;
        Emessage += shiftChar(message[i], charShift);
        if((int)toupper(message[i]) < 65 || (int)toupper(message[i]) > 90){
            numSkips++;
        }
    }

    return Emessage;
}

string getChainEncryption(string message, int method_choice, string key1, string key2, string key_phrase){
    if(method_choice == 1 || method_choice == 2){
        if(key1 == "" || key2 == "" || key_phrase == ""){
            return message;
        }
        string eMessage = getLayer1MessageEncryption(message, method_choice, key1, key2);
        if(eMessage == "--Error--"){
            return message;
        }
        string eMessageSecond = getLayer2MessageEncryption(eMessage, key_phrase);
        if(eMessageSecond == "--Error--"){
            return message;
        }
        return eMessageSecond;
    }else{
        return message;
    }  
}

string getLayer2MessageDecryption(string encrypted_message, string key_phrase){
    int Klength = key_phrase.length();
    int Mlength = encrypted_message.length();
    string encrypt = "";
    int cryptL = 0;

    if(key_phrase == ""){
        return "--Error--";
    } else if(encrypted_message == ""){
        return "--Error--";
    }
    
    for(int i = 0; i < Klength; i++){
        if(((int)toupper(key_phrase[i]) < 65 || (int)toupper(key_phrase[i]) > 90) && key_phrase[i] != ' '){
            return "--Error--";
        } else if(i == 0){
            encrypt += toupper(key_phrase[0]);
        }else if(key_phrase[i] == ' ' && i != Klength-1){
            encrypt += toupper(key_phrase[i+1]);
        }

        cryptL = encrypt.length();
        if(cryptL >= Mlength){
            break;
        }
    }

    int lengthE = encrypt.length();
    int charShift;
    int numSkips = 0;
    string message = "";

    for(int i = 0; i < Mlength; i++){
        charShift = (int)encrypt[((i-numSkips) % (lengthE))] - 65;
        message += shiftChar(encrypted_message[i], -charShift);
        if((int)toupper(message[i]) < 65 || (int)toupper(message[i]) > 90){
            numSkips++;
        }
    }

    return message;
}

string getLayer1MessageDecryption(string encrypted_message, int method_choice, string key1, string key2){
    string decrypt;
    int shift;
    int length = encrypted_message.length();

    if(key1 == "" || key2 == ""){
        return "--Error--";
    }

    if(method_choice == 2){
        shift = getDoubleKeyShiftValue(key1, key2);
        shift *= -1;
        
        for(int i = 0; i < length; i++){
            decrypt += shiftChar(encrypted_message[i], shift);
        }
        return decrypt;
        
    } else if(method_choice == 1){
        shift = -13;
        
        for(int i = 0; i < length; i++){
            decrypt += shiftChar(encrypted_message[i], shift);
        }
        return decrypt;
    }

    return "--Error--"; 
}

string getChainDecryption(string message, int method_choice, string key1, string key2, string key_phrase){
    string dMessage = getLayer2MessageDecryption(message, key_phrase);
    string dMessage2 = getLayer1MessageDecryption(dMessage, method_choice, key1, key2);
    if(dMessage == "--Error--" || dMessage2 == "--Error--"){
        return message;
    }
    return dMessage2;
}

bool validateInt(string inputVar){
    int strLength = inputVar.length();
    if(!(inputVar[0] == '0' || inputVar[0] == '1' || inputVar[0] == '2' || inputVar[0] == '3' ||
        inputVar[0] == '4' || inputVar[0] == '5' || inputVar[0] == '6' || inputVar[0] == '7' ||
        inputVar[0] == '8' || inputVar[0] == '9' || inputVar[0] == '-')){
        return false;
    }
    
    for(int i = 1; i <= strLength-1; i++){
        if(inputVar[i] == '0' || inputVar[i] == '1' || inputVar[i] == '2' || inputVar[i] == '3' ||
            inputVar[i] == '4' || inputVar[i] == '5' || inputVar[i] == '6' || inputVar[i] == '7' ||
            inputVar[i] == '8' || inputVar[i] == '9'){
            continue;
        }else{
            return false;
        }
    }
    return true;
}

bool keyphraseCheck(string keyphrase){
    int Klength = keyphrase.length();
    for(int i = 0; i < Klength; i++){
        if(((int)toupper(keyphrase[i]) < 65 || (int)toupper(keyphrase[i]) > 90) && keyphrase[i] != ' '){
            return true;
        }
    }
    return false;
}

int main(){
    
    string optionNum;
    int intOption = 0;

    string key1 = "";
    string key2 = "";
    string keyphrase = "";
    string method = "";
    string message = "";

    string eChoice;
    int intEChoice;

    int techniqueChoice = 0;
    string techniqueC;

    int methodC = -1;

    string resultMessage;
    int encryptionType = -1;

    while(intOption != 7){
        
        cout << "Please input 1-7 followed by enter to navigate the menu:" << endl;
        cout << "1. Set Encryption Key 1" << endl;
        cout << "2. Set Encryption Key 2" << endl;
        cout << "3. Set Key Phrase" << endl;
        cout << "4. Select Encryption Method" << endl;
        cout << "5. Encrypt Message" << endl;
        cout << "6. Decrypt Message" << endl;
        cout << "7. Exit Program" << endl;

        bool m4finish = false;
        bool techniqueLoop = true;

        getline(cin,optionNum);

        if(!validateInt(optionNum)){
            cout << "Invalid input" << endl;
            continue;
        }
        
        intOption = stoi(optionNum);

        switch (intOption){
            
            case 1:
                cout << "Enter First key:" << endl;
                getline(cin,key1);

                while(key1 == "" || key1 == " "){
                    cout << "Invalid key" << endl;
                    cout << "Enter First key:" << endl;
                    getline(cin,key1);
                }

                cout << "Successfully set first encryption key to " << key1 << endl;
                break;

            case 2:
                cout << "Enter Second key:" << endl;
                getline(cin,key2);

                while(key2 == "" || key2 == " "){
                    cout << "Invalid key" << endl;
                    cout << "Enter Second key:" << endl;
                    getline(cin,key2);
                }

                cout << "Successfully set second encryption key to " << key2 << endl;
                break;

            case 3:
                cout << "Enter keyphrase:" << endl;
                getline(cin, keyphrase);

                while(keyphrase == "" || keyphrase == " " || keyphraseCheck(keyphrase)){
                    cout << "Invalid keyphrase" << endl;
                    cout << "Enter keyphrase:" << endl;
                    getline(cin,keyphrase);
                }
                
                cout << "Successfully set encryption keyphrase to " << keyphrase << endl;
                break;

            case 4:
                
                while(m4finish == false){
                    cout << "Please input 1-3 to decide encryption technique." << endl;
                    cout << "1. Layer 1 Message Encryption" << endl;
                    cout << "2. Layer 2 Message Encryption" << endl;
                    cout << "3. Chain Encryption" << endl;

                    getline(cin,eChoice);

                    if(!validateInt(eChoice)){
                        cout << "Invalid encryption choice" << endl;
                    } else{
                        intEChoice = stoi(eChoice);

                        switch(intEChoice){
                            case 1:
                                cout << "Successfully set encryption type to 1" << endl;
                                while(techniqueLoop == true){
                                    cout << "Please input 1-2 to decide Layer-1 encryption technique." << endl;
                                    cout << "1. Shift by 13" << endl;
                                    cout << "2. Shift by mixed keys" << endl;

                                    getline(cin, techniqueC);
                                    
                                    if(!validateInt(techniqueC)){
                                        cout << "Invalid encryption choice" << endl;
                                        continue;
                                    } 

                                    techniqueChoice = stoi(techniqueC);

                                    if(techniqueChoice != 1 && techniqueChoice != 2){
                                        cout << "Invalid encryption choice" << endl;
                                        continue;
                                    }

                                    if(techniqueChoice == 1){
                                        methodC = 1;
                                        encryptionType = 1;
                                        techniqueLoop = false;
                                    } else{
                                        methodC = 2;
                                        encryptionType = 1;
                                        techniqueLoop = false;
                                    }
                                }

                                m4finish = true;
                                break;
                            case 2:
                                cout << "Successfully set encryption type to 2" << endl;
                                encryptionType = 2;
                                m4finish = true;
                                break;
                            case 3:
                                cout << "Successfully set encryption type to 3" << endl;
                                while(techniqueLoop == true){
                                    cout << "Please input 1-2 to decide Layer-1 encryption technique." << endl;
                                    cout << "1. Shift by 13" << endl;
                                    cout << "2. Shift by mixed keys" << endl;

                                    getline(cin, techniqueC);
                                    
                                    if(!validateInt(techniqueC)){
                                        cout << "Invalid encryption choice" << endl;
                                        continue;
                                    } 

                                    techniqueChoice = stoi(techniqueC);

                                    if(techniqueChoice != 1 && techniqueChoice != 2){
                                        cout << "Invalid encryption choice" << endl;
                                        continue;
                                    }

                                    if(techniqueChoice == 1){
                                        methodC = 1;
                                        encryptionType = 3;
                                        techniqueLoop = false;
                                    } else{
                                        methodC = 2;
                                        encryptionType = 3;
                                        techniqueLoop = false;
                                    }
                                }

                                m4finish = true;
                                break;
                            default:
                                cout << "Invalid encryption choice" << endl;
                                break;
                        }
                    }

                }
                

                break;
            case 5:
                if(encryptionType == 1 || encryptionType == 3){
                    if(key1 == "" || key2 == "" || keyphrase == "" || methodC == -1){
                        cout << "You cannot do this until you set both keys, keyphrase and choose an encryption method" << endl;
                    }else{
                        cout << "Enter your message to encrypt: " << endl;
                        getline(cin, message);
                        
                        if(encryptionType == 1){
                            resultMessage = getLayer1MessageEncryption(message,methodC,key1,key2);
                                if(resultMessage == "--Error--"){
                                resultMessage = message;
                            }

                            cout << resultMessage << endl;

                        }else if(encryptionType == 3){
                            resultMessage = getChainEncryption(message,methodC,key1,key2, keyphrase);
                            

                            cout << resultMessage << endl;

                        }
                    }     
                } else if(encryptionType == 2){ 
                    cout << "Enter your message to encrypt: " << endl;
                    getline(cin, message);
                    resultMessage = getLayer2MessageEncryption(message, keyphrase);

                    if(resultMessage == "--Error--"){
                        resultMessage = message;
                    }

                    cout << resultMessage << endl;
    
                } else{
                    cout << "You cannot do this until you set both keys, keyphrase and choose an encryption method" << endl;
                }
                
                break;
            case 6:
                if(encryptionType == 1 || encryptionType == 3){
                    if(key1 == "" || key2 == "" || keyphrase == "" || methodC == -1){
                        cout << "You cannot do this until you set both keys, keyphrase and choose an encryption method" << endl;
                    }else{
                        cout << "Enter your message to decrypt: " << endl;
                        getline(cin, message);
                        
                        if(encryptionType == 1){
                            resultMessage = getLayer1MessageDecryption(message,methodC,key1,key2);
                            if(resultMessage == "--Error--"){
                                resultMessage = message;
                            }

                            cout << resultMessage << endl;

                        }else if(encryptionType == 3){
                            resultMessage = getChainDecryption(message,methodC,key1,key2, keyphrase);

                            cout << resultMessage << endl;
                        }
                    }     
                } else if(encryptionType == 2){
                    cout << "Enter your message to decrypt: " << endl;
                    getline(cin, message);
                    resultMessage = getLayer2MessageDecryption(message, keyphrase);

                    if(resultMessage == "--Error--"){
                        resultMessage = message;
                    }

                    cout << resultMessage << endl;
                } else{
                    cout << "You cannot do this until you set both keys, keyphrase and choose an encryption method" << endl;
                }
                break;
            case 7:
                cout << "Goodbye." << endl;
                break;
            default:
                cout << "Invalid input" << endl;
                break;
        }

    }
    
    return 0;
}