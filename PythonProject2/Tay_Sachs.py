import re

def check_tay_sachs(sequence):
    # ⃣ בדיקה למוטציה InsTATC1278 (Insertion של TATC)
    ins_region = re.search(r"(.{5,15})TATC(.{5,15})", sequence)  # מחפש TATC עם הקשר גנטי סביבו
    if ins_region:
        return True

    # ⃣ בדיקה למוטציה G269S (G->A) - מחפש את הדפוס סביב המיקום הזה
    g269s_region = re.search(r"(.{10,15})GGCT(.{10,15})", sequence)  # GGCT אמור להיות תקין
    if g269s_region and "AGCT" in sequence:  # מוטציה G->A
        return True

    # ⃣ בדיקה למוטציה 1421+1G>C (Splice Site
    splice_region = re.search(r"(.{10,15})GCCC(.{10,15})", sequence)  # GCCT במקום GCCC
    if splice_region:
        return True

    return False


