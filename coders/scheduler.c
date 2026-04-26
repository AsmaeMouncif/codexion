
# include "codexion.h"

static int is_higher_priority(t_request a, t_request b, char *scheduler)
{
    if (strcmp(scheduler, "fifo") == 0)
        return (a.arrival_time < b.arrival_time);
    else
        return (a.deadline < b.deadline);
}

void heap_push(t_dongle *dongle, t_request req, char *scheduler)
{
    int i;
    int parent;
    t_request tmp;

    i = dongle->heap_size;
    dongle->heap[i] = req;
    dongle->heap_size++;

    while (i > 0)
    {
        parent = (i - 1) / 2;

        if (is_higher_priority(dongle->heap[i], dongle->heap[parent], scheduler))
        {
            tmp = dongle->heap[i];
            dongle->heap[i] = dongle->heap[parent];
            dongle->heap[parent] = tmp;

            i = parent;
        }
        else
            break;
    }
}

void heap_pop(t_dongle *dongle, char *scheduler)
{
    int i;
    int left;
    int right;
    int best;
    t_request tmp;

    if (dongle->heap_size == 0)
        return;

    dongle->heap[0] = dongle->heap[dongle->heap_size - 1];
    dongle->heap_size--;

    i = 0;

    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        best = i;

        if (left < dongle->heap_size &&
            is_higher_priority(dongle->heap[left], dongle->heap[best], scheduler))
            best = left;

        if (right < dongle->heap_size &&
            is_higher_priority(dongle->heap[right], dongle->heap[best], scheduler))
            best = right;

        if (best != i)
        {
            tmp = dongle->heap[i];
            dongle->heap[i] = dongle->heap[best];
            dongle->heap[best] = tmp;

            i = best;
        }
        else
            break;
    }
}

t_request heap_peek(t_dongle *dongle)
{
    t_request empty;

    if (dongle->heap_size == 0)
    {
        memset(&empty, 0, sizeof(t_request));
        return (empty);
    }
    return (dongle->heap[0]);
}
