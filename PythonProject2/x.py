
import re


def count_CGG_repeats(sequence):
    """
    סופר את החזרות הרציפות של 'CGG' ברצף ומחזיר את המספר הארוך ביותר.
    """
    matches = re.finditer(r'(?:CGG)+', sequence)  # איתור כל רצף חזרות רציף
    max_repeats = 0

    for match in matches:
        repeat_length = len(match.group()) // 3  # מחלקים ב-3 כי CGG הוא בן 3 נוקלאוטידים
        max_repeats = max(max_repeats, repeat_length)

    return max_repeats


def check_fragile_x(sequence):
    """
    בודק אם האדם נשא למחלת ה-X השביר בהתאם למספר החזרות הרציפות של CGG.
    """
    num_repeats = count_CGG_repeats(sequence)

    if num_repeats < 45:
        return False
    elif 45 <= num_repeats <= 200:
        return True
    else:
        return "טווח לא תקין (אפשרות לתסמונת מלאה)"

