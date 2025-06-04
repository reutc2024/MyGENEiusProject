import re

# רצף ההתייחסות של הגן HBB (קטע קריטי)
REFERENCE_HBB_REGION = "CTGTGGGGCAAGGTGAACGTGGATGAAGTTGGTGGTGAGGCCCTGGGCAGG"

def check_sickle_cell(sequence):
    """
    מזהה אם המוטציה באנמיה חרמשית קיימת בגן HBB.
    """
    mutated_codon = "GTG"  # ואלין (מוטציה חרמשית)

    # מוודא שהרצף מכיל את האזור הקריטי של HBB
    if REFERENCE_HBB_REGION not in sequence:
        return False  # לא נמצא HBB, לא יכול להיות אנמיה חרמשית

    # חיפוש מוטציה (יכול להיות מסביב רצפים נוספים)
    match = re.search(r"(.{0,50})" + mutated_codon + r"(.{0,50})", sequence)
    if match:
        return True  # נמצאה מוטציה
    return False  # לא נמצאה מוטציה



