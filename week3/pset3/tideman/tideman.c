#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Cycle through all candidates till
        // a vote matches candidate name
        if (!strcmp(candidates[i], name))
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int counter = 1;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = counter; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
        counter++;
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int counter = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j <= candidate_count; j++)
        {
            pair input_pair;
            if (preferences[i][j] > preferences[j][i])
            {
                input_pair.winner = i;
                input_pair.loser = j;
                pairs[counter] = input_pair;
                counter++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                input_pair.winner = j;
                input_pair.loser = i;
                pairs[counter] = input_pair;
                counter++;
            }
        }
    }
    pair_count = counter;
    return;
}

void sort_pairs(void)
{
    int strength[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        strength[i] = preferences[pairs[i].winner][pairs[i].loser];
    }

    bool isSorted;
    do 
    {
        isSorted = true;
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (strength[j] < strength[j + 1])
            {
                isSorted = false;

                pair swap_pair = pairs[j];
                int swap_strength = strength[j];

                pairs[j] = pairs[j + 1];
                strength[j] = strength[j + 1];

                pairs[j + 1] = swap_pair;
                strength[j + 1] = swap_strength;
            }
        }
    }
    while (isSorted);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        int cycle = false;
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;
        
        int counter = 0;
        int node = winner;

        while (counter < candidate_count)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[j][node])
                {
                    if (j == loser)
                    {
                        cycle = true;
                        counter = candidate_count;
                        break;
                    }
                    node = j;
                    continue;
                }
            }
            counter++;
        }
        if (!cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] && winner == j)
            {
                winner = i;
            }
        }
    }
    printf("%s\n", candidates[winner]);
    return;
}
