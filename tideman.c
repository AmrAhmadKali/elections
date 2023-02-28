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
void sort(pair temp[], int l, int r);
void merge(pair temp[], int left_Start, int left_End, int right_Start, int right_End);
bool isCircle(int cycle_start, int loser);
int loserIs(int winnerNo);


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
        if (strcmp(name, candidates[i]) == 0)
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

    for (int i = 0; i < candidate_count - 1; i++) //loops till just before the last array element
    {
        for (int k = i + 1; k < candidate_count; k++)
        {
            if (ranks[k] > ranks[i])
            {
                preferences[ ranks[i] ][ ranks[k] ]++;
            }
            if (ranks[k] < ranks[i])
            {
                preferences[ ranks[i] ][ ranks[k] ]++;
            }

        }

    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;

                pair_count++;
            }

            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;

                pair_count++;
            }

        }

    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp[pair_count];

    sort(temp, 0, pair_count - 1);

}


void sort(pair temp[], int l, int r)
{
    if (r - l <= 0)  //Base case
    {
        return;

    }
    else
    {
        int m = (l + r) / 2;
        sort(temp, l, m);          //left is shorter in case of truncation
        sort(temp, m + 1, r);      //right longer

        merge(temp, l, m, m + 1, r);

    }

}

void merge(pair temp[], int left_Start, int left_End, int right_Start, int right_End)
{
    int i = left_Start; // index into left subarray
    int j = right_Start; // index into right subarray
    int k = left_Start; // index into temp

    while (i <= left_Start && j <= right_Start)
    {
        if (preferences[ pairs[i].winner ][ pairs[i].loser ] > preferences[ pairs[j].winner ][ pairs[j].loser ])
        {
            temp[k].winner = pairs[i].winner;
            temp[k++].loser = pairs[i++].loser;
        }
        else
        {
            temp[k].winner = pairs[j].winner;
            temp[k++].loser = pairs[j++].loser;
        }
    }
    while (i <= left_End) //Copy the remaining elements of left array, if there are any
    {
        temp[k].winner = pairs[i].winner;
        temp[k++].loser = pairs[i++].loser;
    }
    while (j <= right_End) //Copy the remaining elements of right array, if there are any
    {
        temp[k].winner = pairs[j].winner;
        temp[k++].loser = pairs[j++].loser;
    }

    for (i = left_Start; i <= right_End; i++) //for copying the sorted subarray into the original pair array
    {
        pairs[i].winner = temp[i].winner;
        pairs[i].loser = temp[i].loser;
    }

}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!isCircle(pairs[i].winner, pairs[i].loser))
        {
            // Lock the pair unless it makes a circle
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    
}
bool isCircle(int cycle_start, int loser)
{
    if (loser == cycle_start)
    {
        // If the current loser is the cycle start
        // The entry makes a circle
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (isCircle(cycle_start, i) || isCircle(cycle_start, loserIs(i)))
            {
                // Forward progress through the circle
                return true;
            }
        }
        
    }
    return false;
}
int loserIs(int winnerNo)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (pairs[i].winner == winnerNo)
        {
            return pairs[i].loser;
        }
    }

    return -1;    
}
// Print the winner of the election
void print_winner(void)
{
    int count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                break;
            }
            else
            {
                count++;
            }
        }
        if (count == candidate_count)
        {
            printf("%s\n", candidates[i]);
            break;
        }
        else
        {
            count = 0;
        }

    }
}
