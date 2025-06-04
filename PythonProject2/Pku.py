def check_pku(sequence):
    # רשימה של מוטציות נפוצות שמתארות נשאות למחלה
    characteristic_fragments = [
        "GACTGCTGAGG",
        "CTGGAGATGAC",
        "AAGCTGGTGTC",
        "TGGAAGCTCTG"
    ]

    for fragment in characteristic_fragments:
        if fragment in sequence:
            return True

    return False
