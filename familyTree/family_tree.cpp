#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <limits> // בשביל std::numeric_limits
#include <nlohmann/json.hpp>
using json = nlohmann::json;


enum class State { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15, Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25 };

// הגדרת מבנה נתונים עבור מעברים
struct TransitionData {
    State nextState; // מצב הבא
    double percentage; // אחוזים עבור כל מצב
    std::string message;// הודעה עבור כל מצב
    std::vector<char> validInputs; //  וקטור עבור קלטים תקינים
    bool isAcceptingState; //  שדה בוליאני למצב מקבל

};

//State stringToState(const std::string& s) {
//    if (s.length() < 2 || s[0] != 'Q') {
//        throw std::invalid_argument("Invalid state string: " + s);
//    }
//    int stateNum = std::stoi(s.substr(1)); // קבל את המספר אחרי 'Q'
//    if (stateNum >= 0 && stateNum <= 25) { // ודא שהמספר בתחום
//        return static_cast<State>(stateNum);
//    }
//    throw std::invalid_argument("Invalid state number in string: " + s);
//}
//
//bool loadTransitionsFromJSON(const std::string& filename, std::map<State, std::map<char, TransitionData>>& transitions) {
//    std::ifstream inputFile(filename);
//    if (!inputFile.is_open()) {
//        std::cerr << "Unable to open file " << filename << std::endl;
//        return false;
//    }
//
//    try {
//        json j;
//        inputFile >> j; // קריאת כל תוכן ה-JSON לתוך אובייקט j
//
//        for (auto const& [currentStateStr, transitionsForState] : j.items()) {
//            State currentState = stringToState(currentStateStr);
//            for (auto const& [inputStr, transitionDataJson] : transitionsForState.items()) {
//                char input = inputStr[0]; // קבל את התו הראשון של הקלט
//
//                TransitionData data;
//                data.nextState = stringToState(transitionDataJson["nextState"].get<std::string>());
//                data.percentage = transitionDataJson["chance"].get<double>();
//                data.message = transitionDataJson["message"].get<std::string>();
//                data.isAcceptingState = transitionDataJson["isFinal"].get<bool>();
//
//                // קריאת validInputs
//                for (const auto& validCharJson : transitionDataJson["validInputs"]) {
//                    data.validInputs.push_back(validCharJson.get<std::string>()[0]); // קבל את התו הראשון
//                }
//
//                transitions[currentState][input] = data;
//            }
//        }
//        return true;
//    }
//    catch (const json::parse_error& e) {
//        std::cerr << "JSON parse error: " << e.what() << std::endl;
//        return false;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error loading transitions: " << e.what() << std::endl;
//        return false;
//    }
//}
//


//הכנסת פרטי האחים

double calculateSiblingsChance()
{
    std::cout << "Let's start with your siblings -\n";
    while (true)
    {
        unsigned int sickSiblings = 0;
        unsigned int carrierSiblings = 0;
        unsigned int numOfSiblings = 0;
        double siblingsPercent = 0;
        double siblingsChance = 0;

        // פונקצית (למדה) פנימית לאימות קלט מספרי תקין
        auto getUnsignedIntInput = [](const std::string& prompt) -> unsigned int {//auto נותן לקומפיילר להבין לבד את הטיפוס של הפונקציה.

            //[] מסמן שאין שימוש במשתנים חיצוניים(לא "לוכד" כלום
            unsigned int value;
            while (true) {
                std::cout << prompt << std::endl;
                std::cin >> value;

                if (std::cin.fail()) {
                    std::cin.clear(); // מנקה את מצב השגיאה
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // מנקה את שורת הקלט
                    std::cout << "Invalid input. Please enter a whole number only." << std::endl;
                }
                else {
                    return value;
                }
            }
            };

        numOfSiblings = getUnsignedIntInput("Please enter the number of your siblings (if any):");

        if (numOfSiblings == 0)
            return 0.0;

        carrierSiblings = getUnsignedIntInput("Enter the number of your *carrier* siblings:");
        if (carrierSiblings == numOfSiblings) {
            siblingsPercent = 20.0 / numOfSiblings;
            siblingsChance = (siblingsPercent * carrierSiblings)/ 100.0;
            std::cout << "The total chance contribution from siblings is: " << siblingsChance << std::endl;
            return siblingsChance;
        }
        while (carrierSiblings > numOfSiblings) {
            std::cout << "Carrier siblings can't be more than the total number of siblings." << std::endl;
            carrierSiblings = getUnsignedIntInput("Enter the number of your *carrier* siblings:");
        }

        sickSiblings = getUnsignedIntInput("Enter the number of your *sick* siblings:");
        while (sickSiblings > numOfSiblings) {
            std::cout << "Sick siblings can't be more than the total number of siblings." << std::endl;
            sickSiblings = getUnsignedIntInput("Enter the number of your *sick* siblings:");
        }

        if (sickSiblings + carrierSiblings > numOfSiblings) {
            std::cout << "One or more of the inputs entered are incorrect. Please try again.\n" << std::endl;
        }
        else {
            siblingsPercent = 10.0 / numOfSiblings;
            siblingsChance = ((siblingsPercent * sickSiblings) + ((siblingsPercent / 2) * carrierSiblings)) / 100.0;
            std::cout << "The total chance contribution from siblings is: " << siblingsChance << std::endl;
            return siblingsChance;
        }
    }
}


//לבדוק אם החישוב כיצד הוא מתבצע-כי צריך לדאוג שהחישוב לא יעלה על 100 אחוז וגם להעביר את האוטומט לקובץ
int main()
{
    std::map<State, std::map<char, TransitionData>> transitions;
    // טעינת הגדרות האוטומט מקובץ JSON
  // if (!loadTransitionsFromJSON("automaton_transitions.json", transitions)) {
  //     return 1; // יציאה אם הקובץ לא נטען
  // }

    // הגדרת טבלת מעברים עם קלטים תקינים
  transitions[State::Q0]['0'] = { State::Q1, 0.0, "", {'0', '1', '2','U'},false };
  transitions[State::Q0]['1'] = { State::Q2, 0.15, "", {'0', '1', '2','U'},false };
  transitions[State::Q0]['2'] = { State::Q3, 0.20, "", {'0', '1', '2','U'},false };
 
  transitions[State::Q1]['0'] = { State::Q4, 0.0, "Input information for your grandfather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
  transitions[State::Q1]['1'] = { State::Q5, 0.02, "Input information for your grandfather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
 
  transitions[State::Q2]['0'] = { State::Q6, 0.0, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'} ,false };
  transitions[State::Q2]['1'] = { State::Q7, 0.03, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
  transitions[State::Q2]['2'] = { State::Q8, 0.07, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
 
  transitions[State::Q3]['1'] = { State::Q9, 0.05, "Input information for your grandfather- press 1 (carrier) / 2 (sick) or U to change your last choice", { '1', '2'},false };
  transitions[State::Q3]['2'] = { State::Q10, 0.1, "Input information for your grandfather- press 1 (carrier) / 2 (sick) or U to change your last choice", { '1', '2'},false };
 
  transitions[State::Q4]['0'] = { State::Q11, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
  transitions[State::Q4]['1'] = { State::Q12, 0.02, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
 
  transitions[State::Q5]['0'] = { State::Q13, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
  transitions[State::Q5]['1'] = { State::Q14, 0.02, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
 
  transitions[State::Q6]['1'] = { State::Q15, 0.03, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
  transitions[State::Q6]['2'] = { State::Q16, 0.07, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
 
  transitions[State::Q7]['0'] = { State::Q17, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
  transitions[State::Q7]['1'] = { State::Q18, 0.03, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
  transitions[State::Q7]['2'] = { State::Q19, 0.07, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
 
 
  transitions[State::Q8]['0'] = { State::Q20, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
  transitions[State::Q8]['1'] = { State::Q21, 0.03, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
 
  transitions[State::Q9]['1'] = { State::Q22, 0.05, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
  transitions[State::Q9]['2'] = { State::Q23, 0.1, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
 
  transitions[State::Q10]['1'] = { State::Q24, 0.05, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
  transitions[State::Q10]['2'] = { State::Q25, 0.1, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
 
  transitions[State::Q11]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q12]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q13]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q14]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q15]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q16]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q17]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q18]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q19]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q20]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q21]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q22]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q23]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q24]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
  transitions[State::Q25]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };


    double SumTotalPercentage = 0;//סוכם את האחוזים של 2 הנשאים
    std::stack<State> stateHistory; // מחסנית להיסטוריית מצבים
    std::stack<double> percentageHistory; // מחסנית להיסטוריית אחוזים
    double finalCalculation = 0;//החישוב הסופי
    double siblingsChance = 0;//מתאר את אחוז הסיכויים לחלות על ידי האחים


    for (int i = 0;i < 2;i++)

    {
        std::cout << "user number " << i+1 << " please enter the ditails for your family:" << std::endl;
        double siblingsChance = calculateSiblingsChance();
        int parentCount = 0;//סופר את ההורים
        transitions[State::Q0]['0'] = { State::Q1, 0.0, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick) ", {'0', '1', '2'},false };
        transitions[State::Q0]['1'] = { State::Q2, 0.15, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick)", {'0', '1', '2'},false };
        transitions[State::Q0]['2'] = { State::Q3, 0.20, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick)", {'0', '1', '2'},false };
        char input = '1';
        State currentState = State::Q0;
        double totalPercentage = 0;//מחשב את האחוזים עבור כל נשא

        while (parentCount < 2)
        {
            std::cout << transitions[currentState][input].message << std::endl;
            std::cin >> input;

            // חזרה אחורה
            if (input == 'U')
            {
                if (!stateHistory.empty() && !percentageHistory.empty())
                {
                    currentState = stateHistory.top();
                    stateHistory.pop();
                    totalPercentage = percentageHistory.top();
                    percentageHistory.pop();
                    std::cout << "Returned to previous state. Chance so far: " << totalPercentage << std::endl;
					input = '1'; // Reset input to a valid state
                    std::cout << transitions[currentState][input].message << std::endl;
                    std::cin >> input;
                }
                else
                {
                    std::cout << "No previous state to return to." << std::endl;
                    continue;
                }
            }

            // בדיקת קלט תקין לפי הוקטור
            bool validInput = false;
            for (char validChar : transitions[currentState][input].validInputs)
            {
                if (input == validChar)
                {
                    validInput = true;
                    break;
                }
            }
            if (!validInput) {
                std::cout << "Invalid input. Please try again." << std::endl;
                continue;
            }

            // שמירת היסטוריה
            stateHistory.push(currentState);
            percentageHistory.push(totalPercentage);

            TransitionData transitionData = transitions[currentState][input];


            if (currentState == State::Q0)
            {
                if (input == '2')
                {
                    transitions[State::Q0]['0'].validInputs = { '0', '1','U' };
                    transitions[State::Q0]['1'].validInputs = { '0', '1','U' };
                    transitions[State::Q0]['2'].validInputs = { '0', '1','U' };
                }
                else
                    if (input == '0')
                    {
                        transitions[State::Q0]['0'].validInputs = { '1', '2','U' };
                        transitions[State::Q0]['1'].validInputs = { '1', '2','U' };
                        transitions[State::Q0]['2'].validInputs = { '1', '2','U' };
                    }
            }

            currentState = transitionData.nextState;
            totalPercentage += transitionData.percentage;
            bool isAcceptingState = transitions[currentState]['B'].isAcceptingState;
            std::cout << "isAcceptingState: " << isAcceptingState << std::endl;


            std::cout << "Updated chance: " << totalPercentage << std::endl;
            std::cout << "Current state: " << static_cast<int>(currentState) << std::endl;

            if (isAcceptingState)
            {
                SumTotalPercentage += totalPercentage;
                transitions[State::Q0]['0'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
                transitions[State::Q0]['1'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
                transitions[State::Q0]['2'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
                totalPercentage = 0;
                //ריקון המחסניות
                while (!stateHistory.empty()) stateHistory.pop();
                while (!percentageHistory.empty()) percentageHistory.pop();
                currentState = State::Q0;
                parentCount++;
            }

        }

        //totalPercentage = static_cast<int>(totalPercentage) % 2;
        std::cout << "your current state is " << static_cast<int>(currentState) << std::endl;

        //כאן לחשב את הסיכוי הסופי מ2 הנשאים
       // std::cout << "the final chance: " << totalPercentage << std::endl;
        std::cout << "the siblings chance: " << siblingsChance << std::endl;

        //מחשב את האחוז הסופי של הנשא והאחים שלו ומוסיף למשתנה שמשכלל את כל החישוב הסופי
        SumTotalPercentage += (static_cast<int>(totalPercentage) / 2 + static_cast<int>(totalPercentage) % 2) + siblingsChance;
        std::cout << "SumTotalPercentage: " << SumTotalPercentage/2 << "% " << std::endl;
		siblingsChance = 0;//מנקה את האחוזים של האחים
    }

    //חישוב סופי של 2 הנשאים כולל האחים
    finalCalculation = ((SumTotalPercentage/2) * 0.75 + 0.25) * 100.0;
    std::cout << "----------------------------------- "<< std::endl;
    std::cout << "the final calculation is: " << finalCalculation << "% " << std::endl;
    std::cout << "----------------------------------- " << std::endl;

    return 0;
};

//קוד אחרטו רזרבה
//#include <iostream>
//#include <map>
//#include <vector>
//#include <string>
//#include <stack>
//#include <fstream>
//
//
//enum class State { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15, Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25 };
//
//// הגדרת מבנה נתונים עבור מעברים
//struct TransitionData {
//    State nextState; // מצב הבא
//    double percentage; // אחוזים עבור כל מצב
//    std::string message;// הודעה עבור כל מצב
//    std::vector<char> validInputs; //  וקטור עבור קלטים תקינים
//    bool isAcceptingState; //  שדה בוליאני למצב מקבל
//
//};
//
////הכנסת פרטי האחים
//double calculateSiblingsChance()
//{
//    std::cout << "Lets start with your siblings-" << std::endl;
//    while (true)
//    {
//        unsigned int sickSiblings = 0;//אחים חולים
//        unsigned int carrierSiblings = 0;//אחים נשאים
//        unsigned int numOfSiblings = 0;//מספר האחים
//        double siblingsPercent = 0;//משתנה עזר לחישוב יחס האחים
//        double siblingsChance = 0;//מתאר את אחוז הסיכויים לחלות על ידי האחים
//        std::cout << "please enter the number of your siblings (if any)" << std::endl;
//        std::cin >> numOfSiblings;
//        if (numOfSiblings != 0)
//        {
//            std::cout << "Now, please enter the number of your carrier siblings (if any)" << std::endl;
//            std::cin >> carrierSiblings;
//            while (carrierSiblings > numOfSiblings)
//            {
//                std::cout << "Please enter a valid number of carrier siblings (less than or equal to the total number of siblings)" << std::endl;
//                std::cin >> carrierSiblings;
//            }
//            std::cout << "We're almost done! Now just enter the number of your  sick siblings (if any)" << std::endl;
//            std::cin >> sickSiblings;
//            while (sickSiblings > numOfSiblings)
//            {
//                std::cout << "Please enter a valid number of sick siblings (less than or equal to the total number of siblings)" << std::endl;
//                std::cin >> sickSiblings;
//            }
//            if (sickSiblings + carrierSiblings > numOfSiblings)
//            {
//                std::cout << "One or more of the inputs entered are incorrect. Please try again to fill in the details correctly." << std::endl;
//            }
//            else
//            {
//                siblingsPercent = 10.0 / numOfSiblings;
//                siblingsChance = ((siblingsPercent * sickSiblings) + ((siblingsPercent / 2) * carrierSiblings)) / 100.0;
//                std::cout << "the total chance of the siblings is  " << siblingsChance << std::endl;
//                return siblingsChance;
//            }
//        }
//        else
//        {
//            return 0.0;
//        }
//    }
//
//}
//
////לבדוק אם החישוב כיצד הוא מתבצע-כי צריך לדאוג שהחישוב לא יעלה על 100 אחוז וגם להעביר את האוטומט לקובץ
//int main()
//{
//    std::map<State, std::map<char, TransitionData>> transitions;
//    // הגדרת טבלת מעברים עם קלטים תקינים
//    transitions[State::Q0]['0'] = { State::Q1, 0.0, "", {'0', '1', '2','U'},false };
//    transitions[State::Q0]['1'] = { State::Q2, 0.15, "", {'0', '1', '2','U'},false };
//    transitions[State::Q0]['2'] = { State::Q3, 0.20, "", {'0', '1', '2','U'},false };
//
//    transitions[State::Q1]['0'] = { State::Q4, 0.0, "Input information for your grandfather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//    transitions[State::Q1]['1'] = { State::Q5, 0.02, "Input information for your grandfather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//
//    transitions[State::Q2]['0'] = { State::Q6, 0.0, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'} ,false };
//    transitions[State::Q2]['1'] = { State::Q7, 0.03, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//    transitions[State::Q2]['2'] = { State::Q8, 0.07, "Input information for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//
//    transitions[State::Q3]['1'] = { State::Q9, 0.05, "Input information for your grandfather- press 1 (carrier) / 2 (sick) or U to change your last choice", { '1', '2'},false };
//    transitions[State::Q3]['2'] = { State::Q10, 0.1, "Input information for your grandfather- press 1 (carrier) / 2 (sick) or U to change your last choice", { '1', '2'},false };
//
//    transitions[State::Q4]['0'] = { State::Q11, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//    transitions[State::Q4]['1'] = { State::Q12, 0.02, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//
//    transitions[State::Q5]['0'] = { State::Q13, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//    transitions[State::Q5]['1'] = { State::Q14, 0.02, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//
//    transitions[State::Q6]['1'] = { State::Q15, 0.03, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//    transitions[State::Q6]['2'] = { State::Q16, 0.07, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//
//    transitions[State::Q7]['0'] = { State::Q17, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//    transitions[State::Q7]['1'] = { State::Q18, 0.03, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//    transitions[State::Q7]['2'] = { State::Q19, 0.07, "Input information for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//
//
//    transitions[State::Q8]['0'] = { State::Q20, 0.0, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//    transitions[State::Q8]['1'] = { State::Q21, 0.03, "Input information for your grandmather- press 0 (healty) / 1 (carrier) or U to change your last choice", {'0', '1','U'},false };
//
//    transitions[State::Q9]['1'] = { State::Q22, 0.05, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//    transitions[State::Q9]['2'] = { State::Q23, 0.1, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//
//    transitions[State::Q10]['1'] = { State::Q24, 0.05, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//    transitions[State::Q10]['2'] = { State::Q25, 0.1, "Input information for your grandmather- press 1 (carrier) / 2 (sick) or U to change your last choice", {'1', '2','U'},false };
//
//    transitions[State::Q11]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q12]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q13]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q14]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q15]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q16]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q17]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q18]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q19]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q20]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q21]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q22]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q23]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q24]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//    transitions[State::Q25]['B'] = { State::Q0, 0.0, "Input information for your mother- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'B','U'},true };
//
//
//    double SumTotalPercentage = 0;//סוכם את האחוזים של 2 הנשאים
//    std::vector<char> validInputs = { '0', '1', '2', 'U' };
//    std::stack<State> stateHistory; // מחסנית להיסטוריית מצבים
//    std::stack<double> percentageHistory; // מחסנית להיסטוריית אחוזים
//    double finalCalculation = 0;//החישוב הסופי
//    double siblingsChance = 0;//מתאר את אחוז הסיכויים לחלות על ידי האחים
//
//
//    for (int i = 0;i < 2;i++)
//    {
//        double siblingsChance = calculateSiblingsChance();
//        int parentCount = 0;//סופר את ההורים
//        transitions[State::Q0]['0'] = { State::Q1, 0.0, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//        transitions[State::Q0]['1'] = { State::Q2, 0.15, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//        transitions[State::Q0]['2'] = { State::Q3, 0.20, "Input information for your father- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice", {'0', '1', '2','U'},false };
//        char input = '1';
//        State currentState = State::Q0;
//        double totalPercentage = 0;//מחשב את האחוזים עבור כל נשא
//
//        while (parentCount < 2)
//        {
//            std::cout << transitions[currentState][input].message << std::endl;
//            std::cin >> input;
//
//            // חזרה אחורה
//            if (input == 'U')
//            {
//                if (!stateHistory.empty() && !percentageHistory.empty())
//                {
//                    currentState = stateHistory.top();
//                    stateHistory.pop();
//                    totalPercentage = percentageHistory.top();
//                    percentageHistory.pop();
//                    std::cout << "Returned to previous state. Chance so far: " << totalPercentage << std::endl;
//                    continue;
//                }
//                else
//                {
//                    std::cout << "No previous state to return to." << std::endl;
//                    continue;
//                }
//            }
//
//            // בדיקת קלט תקין לפי הוקטור
//            bool validInput = false;
//            for (char validChar : transitions[currentState][input].validInputs)
//            {
//                if (input == validChar)
//                {
//                    validInput = true;
//                    break;
//                }
//            }
//            if (!validInput) {
//                std::cout << "Invalid input. Please try again." << std::endl;
//                continue;
//            }
//
//            // שמירת היסטוריה
//            stateHistory.push(currentState);
//            percentageHistory.push(totalPercentage);
//
//            TransitionData transitionData = transitions[currentState][input];
//
//
//            if (currentState == State::Q0)
//            {
//                if (input == '2')
//                {
//                    transitions[State::Q0]['0'].validInputs = { '0', '1','U' };
//                    transitions[State::Q0]['1'].validInputs = { '0', '1','U' };
//                    transitions[State::Q0]['2'].validInputs = { '0', '1','U' };
//                }
//                else
//                    if (input == '0')
//                    {
//                        transitions[State::Q0]['0'].validInputs = { '1', '2','U' };
//                        transitions[State::Q0]['1'].validInputs = { '1', '2','U' };
//                        transitions[State::Q0]['2'].validInputs = { '1', '2','U' };
//                    }
//            }
//
//            currentState = transitionData.nextState;
//            totalPercentage += transitionData.percentage;
//            bool isAcceptingState = transitions[currentState]['B'].isAcceptingState;
//            std::cout << "isAcceptingState: " << isAcceptingState << std::endl;
//
//
//            std::cout << "Updated chance: " << totalPercentage << std::endl;
//            std::cout << "Current state: " << static_cast<int>(currentState) << std::endl;
//
//            if (isAcceptingState)
//            {
//                SumTotalPercentage += totalPercentage;
//                transitions[State::Q0]['0'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
//                transitions[State::Q0]['1'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
//                transitions[State::Q0]['2'].message = "Input information for your mather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
//                totalPercentage = 0;
//                //ריקון המחסניות
//                while (!stateHistory.empty()) stateHistory.pop();
//                while (!percentageHistory.empty()) percentageHistory.pop();
//                currentState = State::Q0;
//                parentCount++;
//            }
//
//        }
//
//
//        std::cout << "the Chance included by far is: " << totalPercentage << std::endl;
//        std::cout << "your current state is " << static_cast<int>(currentState) << std::endl;
//
//        //כאן לחשב את הסיכוי הסופי מ2 הנשאים
//        std::cout << "the final chance: " << totalPercentage << std::endl;
//
//        //מחשב את האחוז הסופי של הנשא והאחים שלו ומוסיף למשתנה שמשכלל את כל החישוב הסופי
//        SumTotalPercentage += totalPercentage / 2.0 + siblingsChance;
//        std::cout << "SumTotalPercentage: " << SumTotalPercentage << std::endl;
//
//    }
//
//    //חישוב סופי של 2 הנשאים כולל האחים
//    finalCalculation = (SumTotalPercentage * 0.75 + 0.25) * 100.0;
//    std::cout << "the final calculation is: " << finalCalculation << "% " << std::endl;
//
//    return 0;
//};


//int main() {
//    std::map<State, std::map<char, State>> transition;
//    std::map<State, std::map<char, double>> percentages;
//    std::map<State, std::map<char, string>> masseges;
// 
//    // הגדרת טבלת מעברים
//    transition[Q0]['0'] = Q1;
//    transition[Q0]['1'] = Q2;
//    transition[Q0]['2'] = Q3;
//    transition[Q1]['0'] = Q4;
//    transition[Q1]['1'] = Q5;
//    transition[Q1]['2'] = Q6;
//    transition[Q2]['0'] = Q7;
//    transition[Q2]['1'] = Q8;
//    transition[Q2]['2'] = Q9;
//    transition[Q3]['0'] = Q10;
//    transition[Q3]['1'] = Q11;
//    transition[Q3]['2'] = Q12;
//    transition[Q4]['B'] = Q1;
//    transition[Q5]['B'] = Q1;
//    transition[Q6]['B'] = Q1;
//    transition[Q7]['B'] = Q2;
//    transition[Q8]['B'] = Q2;
//    transition[Q9]['B'] = Q2;
//    transition[Q10]['B'] = Q3;
//    transition[Q11]['B'] = Q3;
//    transition[Q12]['B'] = Q3;
//   // transition[Q5]['A'] = Q0;
//   // transition[Q6]['A'] = Q0;
//   // transition[Q7]['A'] = Q0;
//   // transition[Q8]['A'] = Q0;
//   // transition[Q9]['A'] = Q0;
//   // transition[Q10]['A'] = Q0;
//   // transition[Q11]['A'] = Q0;
//   // transition[Q12]['A'] = Q0;
//
//    // הגדרת אחוזים
//    percentages[Q1]['0'] = 0.0;
//    percentages[Q2]['1'] = 0.15;
//    percentages[Q3]['2'] = 0.20;
//    percentages[Q4]['0'] = 0.0;
//    percentages[Q5]['1'] = 0.02;
//    percentages[Q6]['2'] = 0.05;
//    percentages[Q7]['0'] = 0.0;
//    percentages[Q8]['1'] = 0.03;
//    percentages[Q9]['2'] = 0.07;
//    percentages[Q10]['0'] = 0.0;
//    percentages[Q11]['1'] = 0.05;
//    percentages[Q12]['2'] = 0.10;
//
//
//    //// הגדרת טבלת מעברים
//    //transition[Q0]['0'] = Q1;
//    //transition[Q0]['1'] = Q2;
//    //transition[Q0]['2'] = Q3;
//    //transition[Q1]['0'] = Q4;
//    //transition[Q1]['1'] = Q5;
//    //transition[Q1]['2'] = Q6;
//    //transition[Q2]['0'] = Q7;
//    //transition[Q2]['1'] = Q8;
//    //transition[Q2]['2'] = Q9;
//    //transition[Q3]['0'] = Q10;
//    //transition[Q3]['1'] = Q11;
//    //transition[Q3]['2'] = Q12;
//    //transition[Q4]['A'] = Q1;
//    //transition[Q5]['A'] = Q1;
//    //transition[Q6]['A'] = Q1;
//    //transition[Q7]['A'] = Q2;
//    //transition[Q8]['A'] = Q2;
//    //transition[Q9]['A'] = Q2;
//    //transition[Q10]['A'] = Q3;
//    //transition[Q11]['A'] = Q3;
//    //transition[Q12]['A'] = Q3;
//    //transition[Q5]['B'] = Q0;
//    //transition[Q6]['B'] = Q0;
//    //transition[Q7]['B'] = Q0;
//    //transition[Q8]['B'] = Q0;
//    //transition[Q9]['B'] = Q0;
//    //transition[Q10]['B'] = Q0;
//    //transition[Q11]['B'] = Q0;
//    //transition[Q12]['B'] = Q0;
//
//   //// הגדרת אחוזים
//   // percentages[Q0]['0'] = 0.0;
//   // percentages[Q0]['1'] = 0.15;
//   // percentages[Q0]['2'] = 0.20;
//   // percentages[Q1]['0'] = 0.0;
//   // percentages[Q1]['1'] = 0.02;
//   // percentages[Q1]['2'] = 0.05;
//   // percentages[Q2]['0'] = 0.0;
//   // percentages[Q2]['1'] = 0.03;
//   // percentages[Q2]['2'] = 0.07;
//   // percentages[Q3]['0'] = 0.0;
//   // percentages[Q3]['1'] = 0.05;
//   // percentages[Q3]['2'] = 0.10;
//
//   
//    State next;
//    double SumTotalPercentage=0;//סוכם את האחוזים של 2 הנשאים
//    std::vector<char> validInputs={ '0', '1', '2', 'U' };  
//    std::stack<State> stateHistory; // מחסנית להיסטוריית מצבים
//    std::stack<double> percentageHistory; // מחסנית להיסטוריית אחוזים
//    char input;
//    double finalCalculation = 0;//החישוב הסופי
//    
//    
//for(int i=0;i<2;i++){
//    int sickSiblings = 0;//אחים חולים
//    int carrierSiblings = 0;//אחים נשאים
//    int numOfSiblings = 0;//מספר האחים
//    double siblingsPercent = 0;//משתנה עזר לחישוב יחס האחים
//    double siblingsChance = 0;//מתאר את אחוז הסיכויים לחלות על ידי האחים
//    int parentCount = 0;
//    State currentState = Q0;
//    string parent = "Father";
//    double totalPercentage = 0;//מחשב את האחוזים עבור כל נשא
//    std::cout << "Lets start with your siblings, please enter the number of your siblings" << std::endl;
//    std::cin >> numOfSiblings;
//    std::cout << "Now, please enter the number of your carrier siblings (if any)" << std::endl;
//    std::cin >> carrierSiblings;
//    std::cout << "We're almost done! Now just enter the number of your  sick siblings (if any)" << std::endl;
//    std::cin >> sickSiblings;
//    siblingsPercent = 10.0 /numOfSiblings;
//    std::cout << siblingsChance << std::endl;
//    siblingsChance = ((siblingsPercent*sickSiblings)+((siblingsPercent/2)* carrierSiblings))/100.0;
//    std::cout << "the total chance of the siblings is  " << siblingsChance << std::endl;
//    while (parentCount < 2) 
//    {// לוודא שהאוטומט פועל עבור 2 ההורים מ2 הצדדים 
//           
//        if (currentState == Q0) {
//            transition[Q4]['B'] = Q1;
//            transition[Q5]['B'] = Q1;
//            transition[Q6]['B'] = Q1;
//            transition[Q7]['B'] = Q2;
//            transition[Q8]['B'] = Q2;
//            transition[Q9]['B'] = Q2;
//            transition[Q10]['B'] = Q3;
//            transition[Q11]['B'] = Q3;
//            transition[Q12]['B'] = Q3;
//			if (parentCount == 1) 
//                parent = "Mother";
//            std::cout << "Input infornation for your "<< parent<< "- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice  " << std::endl;
//            std::cin >> input;
//            next = transition[currentState][input];
//        }
//
//        else if (currentState == Q1 || currentState == Q2 || currentState == Q3) {
//            std::cout << "Input infornation for your grandfather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice  " << std::endl;
//            std::cin >> input;
//            next = transition[currentState][input];
//            transition[next]['B'] = Q0;       
//        }
//
//        
//        else if (currentState == Q4 || currentState == Q5 || currentState == Q6 || currentState == Q7 || currentState == Q8 || currentState == Q9 || currentState == Q10 || currentState == Q12) 
//        {
//            std::cout << "Input infornation for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice  " << std::endl;
//            std::cin >> input;
//            totalPercentage += percentages[currentState][input];
//            next = transition[currentState][input];
//        }
//
//      //  else
//      //  {
//      //      std::cout << "Input infornation for your grandmather- press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice  ";
//      //      std::cin >> input;
//      //      next = transition[currentState][input];
//      //  }
//
//        if (input == 'U') { // חזרה אחורה
//            if (!stateHistory.empty()) {
//                currentState = stateHistory.top();
//                stateHistory.pop();
//                totalPercentage = percentageHistory.top();
//                percentageHistory.pop();
//                std::cout << "Return to previous state. Chance included: " << totalPercentage << std::endl;
//                continue;
//            }
//            else {
//                std::cout << "There is no an option to return back." << std::endl;
//                continue;
//            }
//        }
//
//        // בדיקת תקינות קלט
//        bool validInput = false;
//        for (char validChar : validInputs) {
//            if (input == validChar) {
//                validInput = true;
//                break;
//            }
//        }
//
//        if (!validInput) {
//            std::cout << "invalid input.please try again" << std::endl;
//            continue;
//        }
//
//        // שמירת היסטוריה
//        stateHistory.push(currentState);
//        percentageHistory.push(totalPercentage);
//
//        currentState = transition[currentState][input];
//
//        if (currentState != Q0) {
//            totalPercentage += percentages[currentState][input];
//        }
//        
//
//        if (currentState == Q0) {
//            parentCount++;
//        }
//
//        std::cout << "the Chance included by far is: " << totalPercentage << std::endl;
//        std::cout << "your current state is " << currentState << std::endl;
//    }
//	//כאן לחשב את הסיכוי הסופי מ2 הנשאים
//    std::cout << "the final chance: " << totalPercentage << std::endl;
//
//    //מחשב את האחוז הסופי של הנשא והאחים שלו ומוסיף למשתנה שמשכלל את כל החישוב הסופי
//    SumTotalPercentage += totalPercentage/2.0 + siblingsChance;
//    std::cout << "SumTotalPercentage: " << SumTotalPercentage << std::endl;
//
//    //ריקון המחסניות
//    while (!stateHistory.empty()) {
//        stateHistory.pop();
//    }
//    while (!percentageHistory.empty()) {
//        percentageHistory.pop();
//    }
//}
//    
//   //חישוב סופי של 2 הנשאים כולל האחים
//   finalCalculation = (SumTotalPercentage*0.75 + 0.25)*100.0;
//   std::cout << "the final calculation is: " << finalCalculation << "% " << std::endl;
//   
//    return 0;
//}



//int main() {
//    std::map<State, std::map<char, State>> transition;
//    std::map<State, std::map<char, double>> percentages;
//
//    // הגדרת טבלת מעברים
//    transition[Q1]['0'] = Q2;
//    transition[Q1]['1'] = Q3;
//    transition[Q1]['2'] = Q4;
//    transition[Q2]['B'] = Q5;
//    transition[Q3]['B'] = Q9;
//    transition[Q4]['B'] = Q11;
//    transition[Q5]['0'] = Q6;
//    transition[Q5]['1'] = Q7;
//    transition[Q5]['2'] = Q12;
//    transition[Q9]['0'] = Q8;
//    transition[Q9]['1'] = Q7;
//    transition[Q9]['2'] = Q10;
//    transition[Q11]['0'] = Q11;
//    transition[Q11]['1'] = Q12;
//    transition[Q11]['2'] = Q12;
//    transition[Q6]['B'] = Q1;
//    transition[Q7]['B'] = Q1;
//    transition[Q8]['B'] = Q1;
//    transition[Q10]['B'] = Q1;
//    transition[Q12]['B'] = Q1;
//
//    // הגדרת אחוזים
//    percentages[Q1]['0'] = 0.0;
//    percentages[Q1]['1'] = 0.15;
//    percentages[Q1]['2'] = 0.20;
//    percentages[Q5]['0'] = 0.0;
//    percentages[Q5]['1'] = 0.02;
//    percentages[Q5]['2'] = 0.05;
//    percentages[Q9]['0'] = 0.0;
//    percentages[Q9]['1'] = 0.03;
//    percentages[Q9]['2'] = 0.07;
//    percentages[Q11]['0'] = 0.0;
//    percentages[Q11]['1'] = 0.05;
//    percentages[Q11]['2'] = 0.10;
//
//    State currentState = Q1;
//    double totalPercentage = 0.25;
//    std::vector<char> validInputs;
//    int parentCount = 0;
//    std::stack<State> stateHistory; // מחסנית להיסטוריית מצבים
//    std::stack<double> percentageHistory; // מחסנית להיסטוריית אחוזים
//
//    while (parentCount < 2) {
//        validInputs.clear();
//
//        if (currentState == Q1) {
//            validInputs = { '0', '1', '2', 'U' }; // הוספת 'U' לחזרה אחורה
//            std::cout << "press 0 (healty) / 1 (carrier) / 2 (sick) or U to change your last choice";
//        }
//        else if (currentState == Q2 || currentState == Q3 || currentState == Q4) {
//            validInputs = { 'B', 'U' }; // הוספת 'U' לחזרה אחורה
//            std::cout << "press B to input the state of your grandFather or U to change your last choice ";
//        }
//        else if (currentState == Q6 || currentState == Q7 || currentState == Q8 || currentState == Q10 || currentState == Q12) {
//            validInputs = { 'B', 'U' }; // הוספת 'U' לחזרה אחורה
//            std::cout << "press B to input the state of your grandFather or U to change your last choice ";
//        }
//        else {
//            validInputs = { '0', '1', '2', 'U' }; // הוספת 'U' לחזרה אחורה
//            std::cout << "הזן 0 (בריא), 1 (נשא), 2 (חולה) או U לחזרה אחורה עבור סבא/סבתא: ";
//        }
//
//        char input;
//        std::cin >> input;
//
//        if (input == 'U') { // חזרה אחורה
//            if (!stateHistory.empty()) {
//                currentState = stateHistory.top();
//                stateHistory.pop();
//                totalPercentage = percentageHistory.top();
//                percentageHistory.pop();
//                std::cout << "Return to previous state. Chance included: " << totalPercentage << std::endl;
//                continue;
//            }
//            else {
//                std::cout << "There is no an option to return back." << std::endl;
//                continue;
//            }
//        }
//
//        // בדיקת תקינות קלט
//        bool validInput = false;
//        for (char validChar : validInputs) {
//            if (input == validChar) {
//                validInput = true;
//                break;
//            }
//        }
//
//        if (!validInput) {
//            std::cout << "invalid input.please try again" << std::endl;
//            continue;
//        }
//
//        // שמירת היסטוריה
//        stateHistory.push(currentState);
//        percentageHistory.push(totalPercentage);
//
//        if (currentState != Q1) {
//            totalPercentage += percentages[currentState][input];
//        }
//        currentState = transition[currentState][input];
//
//        if (currentState == Q1) {
//            parentCount++;
//        }
//
//        std::cout << "the Chance included by far is: " << totalPercentage << std::endl;
//    }
//
//    std::cout << "the final Chance: " << totalPercentage << std::endl;
//
//    return 0;
//}



//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//
////  Enum הגדרת מצב גנטי 
//enum GeneticStatus {
//    Healthy,    // בריא
//    Carrier,    // נשא
//    Affected    // חולה
//};
//
//// פונקציה להמרת enum למחרוזת
//std::string toString(GeneticStatus status) {
//    switch (status) {
//    case Healthy:   return "Healthy";
//    case Carrier:   return "Carrier";
//    case Affected:  return "Affected";
//    default:        return "Unknown";
//    }
//}
//
//// מחלקה לצומת בעץ משפחה
//class FamilyNode {
//public:
//    GeneticStatus status;                  // סטטוס גנטי של האדם
//    std::vector<FamilyNode*> relatives;    // מצביעים להורים ולסבים
//
//
//    // בנאי
//    FamilyNode(GeneticStatus stat) : status(stat) {}
//
//    // פונקציה לקבלת קלט מהמשתמש
//    static GeneticStatus getUserInput(const std::string& person) {
//        int choice;
//        std::cout << "Enter genetic status for " << person << ":\n";
//        std::cout << "0 - Healthy, 1 - Carrier, 2 - Affected: ";
//        std::cin >> choice;
//
//        while (choice < 0 || choice > 2) {
//            std::cout << "Invalid input, please try again: ";
//            std::cin >> choice;
//        }
//
//        return static_cast<GeneticStatus>(choice);
//    }
//
//    // (הוספת קרוב משפחה (הורים, סבים
//    void addRelative(FamilyNode* relative) {
//        relatives.push_back(relative);
//    }
//
//       // הדפסת העץ 
//    void printTree(int level = 0) {
//        // הדפסת האדם עצמו
//        for (int i = 0; i < level; i++) std::cout << "    ";
//        std::cout << toString(status) << std::endl;
//
//        // הדפסת קרובי המשפחה
//        for (FamilyNode* relative : relatives) {
//            relative->printTree(level + 1);
//        }
//    }
//};
//
//int main() {
//
//    
//    // יצירת הצומת של האדם עצמו והוספת אחים
//    FamilyNode* selfNode = new FamilyNode(Carrier);
//
//    // יצירת הורים
//    FamilyNode* father = new FamilyNode(FamilyNode::getUserInput("Father"));
//    FamilyNode* mother = new FamilyNode(FamilyNode::getUserInput("Mother"));
//
//    // יצירת סבים וסבתות מצד אב
//    FamilyNode* paternalGrandfather = new FamilyNode(FamilyNode::getUserInput("Paternal Grandfather"));
//    FamilyNode* paternalGrandmother = new FamilyNode(FamilyNode::getUserInput("Paternal Grandmother"));
//    
//
//    // יצירת סבים וסבתות מצד אם
//    FamilyNode* maternalGrandfather = new FamilyNode(FamilyNode::getUserInput("Maternal Grandfather"));
//    FamilyNode* maternalGrandmother = new FamilyNode(FamilyNode::getUserInput("Maternal Grandmother"));
//    
//
//    // בניית העץ המשפחתי
//    selfNode->addRelative(father);
//    selfNode->addRelative(mother);
//    father->addRelative(paternalGrandfather);
//    father->addRelative(paternalGrandmother);
//    mother->addRelative(maternalGrandfather);
//    mother->addRelative(maternalGrandmother);
//
//    // הדפסת העץ המשפחתי
//    std::cout << "\nFamily Tree:\n";
//    selfNode->printTree();
//
//    // ניקוי זיכרון (!)
//    delete selfNode;
//    delete father;
//    delete mother;
//    delete paternalGrandfather;
//    delete paternalGrandmother;
//    delete maternalGrandfather;
//    delete maternalGrandmother;
//
//    return 0;
//}
