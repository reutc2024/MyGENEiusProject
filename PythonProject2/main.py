import os
import subprocess
from Bio import SeqIO
import Sickle_Cell_Anemia
import Tay_Sachs
#import x
import Cystic_Fibrosis
import Pku
#להוסיף חיבור לC++
#להוסיף סדר עדיפיות למחלות

results_girl = {}
results_boy = {}



def main():
    sequence_girl = get_genetic_sequence()
    if sequence_girl is None:
        print("Failed to read genetic sequence. Exiting.")
        return

    results_girl = findCarrier(sequence_girl)
    result(results_girl)

    if True in results_girl.values():
        sequence_boy = get_genetic_sequence()
        if sequence_boy is None:
            print("Failed to read boy's genetic sequence. Exiting.")
            return

        results_boy = findCarrier(sequence_boy)
        result(results_boy)
        compare_results(results_girl, results_boy)  # שולחת את התוצאות לפונקציה במקום להשתמש במשתנים גלובליים


def findCarrier(sequence):
    """
    בודקת נשאות למחלות שונות על פי רצף DNA ומחזירה מילון עם התוצאות.
    """
    return {
        "Sickle Cell Anemia": Sickle_Cell_Anemia.check_sickle_cell(sequence),
        "Tay Sachs": Tay_Sachs.check_tay_sachs(sequence),
        "Pku": Pku.check_pku(sequence),
        "Cystic Fibrosis": Cystic_Fibrosis.find_cftr_mutations(sequence)
    }


def result(results):
    """
    מדפיסה את תוצאות בדיקת הנשאות של אדם (בת או בן).
    """
    print("\nCarrier status:")
    for disease, is_carrier in results.items():
        print(f"{disease}: {'Carrier' if is_carrier else 'Not Carrier'}")

    if True in results.values():
        print("\nThis individual is a carrier for at least one disease.")
    else:
        print("\nThis individual is not a carrier for those diseases.")


def compare_results(results_girl, results_boy):
    """
    משווה את תוצאות הנשאות של הבת והבן ומזהירה אם שניהם נשאים לאותה מחלה.
    """
    common_diseases = [disease for disease in results_girl if results_girl[disease] and results_boy.get(disease)]

    if common_diseases:
        print("\nWarning: Both individuals are carriers for the following diseases:")
        for disease in common_diseases:
            print(f"- {disease}")
        print("In order to calculate the results, please enter data about your family. ")
        run_automaton_calculator();
    else:
        print("\nThere is no matching carrier status between the two individuals.")

def run_automaton_calculator():
    executable_name = "C:\\Users\\ADMIN\\Documents\\gene_project\\familyTree\\x64\\Debug\\familyTree.exe";
    if not os.path.exists(executable_name):
        print(f"Error: C++ executable '{executable_name}' not found.")
        print("Please ensure you have compiled your C ++ code and the .exe file is in the correct location.")
        return

    print(f"Running C++ program: {executable_name}")

    try:
        # Option 1: Run and allow user interaction directly in the console
        # This is suitable if you want the user to type input directly to the C++ program.
        # The output of the C++ program will also be printed directly to the console.
        print("\n--- Starting C++ program (interactive mode) ---")
        process = subprocess.run([executable_name], check=True, text=True, capture_output=False)
        print("--- C++ program finished ---")

        # Option 2: Run and capture output (and optionally provide input)
        # Use this if you want to control input/output from Python
        # print("\n--- Starting C++ program (capture output mode) ---")
        # Example: Providing some dummy input for testing (replace with actual input logic)
        # You'll need to know the exact sequence of inputs your C++ program expects.
        # user_inputs = "0\n0\nB\n1\n1\nB\n" # Example: 0->0->B (father), 1->1->B (mother)
        # process = subprocess.run([executable_name],
        #  input=user_inputs, # Input needs to be a string (text=True) or bytes (text=False)
        #  capture_output=True, # Capture stdout and stderr
        #  text=True, # Decode stdout/stderr as text using default encoding
        #  check=True # Raise an exception if the process returns a non-zero exit code
        #)
        # print("--- C++ program finished ---")
        #
        # print("\nC++ program output:")
        # print(process.stdout)
        # if process.stderr:
        #     print("\nC++ program errors (stderr):")
        #     print(process.stderr)
        #
        # # You can now parse the output if it contains the final calculation
        # # For example, if the C++ program prints "the final calculation is: XX%" on the last line:
        # output_lines = process.stdout.strip().split('\n')
        # if output_lines:
        #     last_line = output_lines[-1]
        #     if "the final calculation is:" in last_line:
        #         try:
        #             final_calc_str = last_line.split(":")[1].strip().replace('%', '')
        #             final_percentage = float(final_calc_str)
        #             print(f"\nFinal calculation extracted from C++: {final_percentage}%")
        #         except ValueError:
        #             print("Could not parse final calculation from C++ output.")


    except subprocess.CalledProcessError as e:
        print(f"Error running C++ program: {e}")
        print(f"Stdout: {e.stdout}")
        print(f"Stderr: {e.stderr}")
    except FileNotFoundError:
        print(f"Error: The executable '{executable_name}' was not found. "
              "Make sure the path is correct and the C++ program is compiled.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


def read_fasta(file_path):
    """
    קוראת קובץ FASTA ובודקת שהוא מכיל בדיוק רשומה אחת.
    אם יש יותר מרשומה אחת – מחזירה שגיאה.
    """
    with open(file_path, "r") as file:
        records = list(SeqIO.parse(file, "fasta"))  # המרת הגנרטור לרשימה כדי לספור את הרשומות

        if len(records) == 0:
            print("Error: The FASTA file is empty. Please provide a valid file with one sequence.")
            return None
        elif len(records) > 1:
            print("Error: The FASTA file contains multiple sequences. Please upload a file with only one sequence.")
            return None

        return str(records[0].seq)  # מחזיר את רצף ה-DNA של הרשומה היחידה


def get_genetic_sequence():
    """
    מבקשת מהמשתמש נתיב לקובץ FASTA ומחזירה את רצף ה-DNA אם נמצא.
    """
    fasta_file = input("Enter the path to the FASTA file: ").strip()
    fasta_file = os.path.abspath(fasta_file)

    if not os.path.exists(fasta_file):
        print(f"Error: The file does not exist: {fasta_file}")
        return None

    try:
        return read_fasta(fasta_file)
    except Exception as e:
        print(f"Error reading the FASTA file: {e}")
        return None


if __name__ == "__main__":
    main()
