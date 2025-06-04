
def find_cftr_mutations(dna_sequence):
    # רשימת מוטציות נפוצות בגן CFTR
    known_mutations = {
        "CFTR": ["TTGGTGGTG"],
        "AURKA": ["ATGCGT", "GCTTGA"],
        "TUBG1": ["CCGATT", "TTAAGC"],
        "BUB1": ["GGTACA", "TGCATC"]
    }

    carriers = []  # רשימת המוטציות שנמצאו

    for gene, mutations in known_mutations.items():
        for mutation in mutations:
            if mutation in dna_sequence:
                carriers.append(mutation)

    if carriers:

        return True  # האדם נשא של CF
    else:

        return False  # לא נמצא נשא




