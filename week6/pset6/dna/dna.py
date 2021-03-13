import sys
import csv


def main():
    if (len(sys.argv) != 3):
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)

    database = {}
    with open(sys.argv[1]) as database_file:
        reader = csv.DictReader(database_file)
        for i, row in enumerate(reader):
            if i == 0:
                list_of_strs = list(row.keys())[1:]

            name_strs = {}
            for STR in list_of_strs:
                name_strs.update({STR: int(row[STR])})
                
            database[row["name"]] = name_strs

    with open(sys.argv[2]) as sequence_file:
        sequence = sequence_file.read()

    results = processSequence(list_of_strs, sequence)
    match(database, results)


def match(database, results):
    for name in database:
        isMatch = True
        for STR in database[name]:
            if results[STR] != database[name][STR]:
                isMatch = False
                break

        if isMatch:
            print(name)
            return

    print("No match")


def processSequence(list_of_strs, sequence):
    results = {}
    for STR in list_of_strs:
        results[STR] = longestRepeat(STR, sequence)

    return results


def longestRepeat(STR, sequence):
    len_seq = len(sequence)
    bases = list(sequence)

    len_str = len(STR)
    str_bases = list(STR)

    max_repeats = 0
    for i in range(len_seq):
        start = True
        for j in range(len_str):
            if i+j >= len_seq or bases[i+j] != str_bases[j]:
                start = False
                break

        if start:
            repeats = 0
            for a in range(i, len_seq, len_str):
                cont = True
                for b in range(len_str):
                    if a+b >= len_seq or bases[a+b] != str_bases[b]:
                        cont = False
                        break
                if cont:
                    repeats += 1
                else:
                    break

            if repeats > max_repeats:
                max_repeats = repeats

    return max_repeats


if __name__ == "__main__":
    main()
