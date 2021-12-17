#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define ARR_SIZE 2048
#define MAX_LIST_LEN 64
#define LITERAL_ID 4

#define SUM 0
#define PRODUCT 1
#define MIN 2
#define MAX 3
#define GREATER_THAN 5
#define LESS_THAN 6
#define EQUAL 7

static int version_sum = 0;

typedef struct list_t
{
    uint64_t *arr;
    int size;
    int _alloc_size;
} list_t;

typedef struct bitstream_t
{
    uint8_t *arr;
    int size;
    int bit_offset;
    int arr_offset;
} bitstream_t;

uint16_t get_n_next_bits(bitstream_t *stream, uint8_t n);
uint64_t parse_packet(bitstream_t *stream);
uint64_t parse_literal(bitstream_t *stream);
uint64_t parse_operator(bitstream_t *stream, uint8_t id);
void append(list_t *list, uint64_t val);
uint64_t op(list_t *list, uint8_t id);

int main()
{
    FILE *fp = fopen("./inputs.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file");
        fclose(fp);
        exit(1);
    }

    uint8_t _stream_arr[ARR_SIZE];
    bitstream_t stream =
    {
        .arr = _stream_arr,
        .size = 0,
        .bit_offset = 0,
        .arr_offset = 0,
    };

    while (fscanf(fp, "%2hhx", &(stream.arr[stream.size++])) != EOF && stream.size < ARR_SIZE);
    fclose(fp);

    uint64_t result = parse_packet(&stream);
    printf("Sum of version numbers: %d\n", version_sum);
    printf("Packet result: %ld\n", result);

    return 0;
}

uint64_t parse_packet(bitstream_t *stream)
{
    uint16_t version = get_n_next_bits(stream, 3);
    uint16_t id = get_n_next_bits(stream, 3);

    version_sum += version;

    if (id == LITERAL_ID)
        return parse_literal(stream);
    return parse_operator(stream, id);
}

uint64_t parse_literal(bitstream_t *stream)
{
    uint64_t literal = 0;
    while (get_n_next_bits(stream, 1) == 1)
    {
        uint16_t sub = get_n_next_bits(stream, 4);
        literal = (literal << 4) | sub;
    }

    uint16_t sub = get_n_next_bits(stream, 4);
    literal = (literal << 4) | sub;

    return literal;
}

uint64_t parse_operator(bitstream_t *stream, uint8_t id)
{
    list_t list = 
    {
        .arr = malloc(2 * sizeof(uint64_t)),
        .size = 0,
        ._alloc_size = 2
    };

    uint16_t mode = get_n_next_bits(stream, 1);
    if (mode)
    {
        uint16_t num_packets = get_n_next_bits(stream, 11);
        for (int i = 0; i < num_packets; i++)
        {
            append(&list, parse_packet(stream));
        }
    }
    else
    {
        uint16_t total_len = get_n_next_bits(stream, 15);
        int starting_index = stream->arr_offset * sizeof(*(stream->arr)) * 8 + stream->bit_offset;
        while (starting_index + total_len > stream->arr_offset * sizeof(*(stream->arr)) * 8 + stream->bit_offset)
        {
            append(&list, parse_packet(stream));
        }
    }

    uint64_t result = op(&list, id);
    free(list.arr);
    return result;
}

uint64_t op(list_t *list, uint8_t id)
{
    switch (id)
    {
    case SUM:
    {
        uint64_t result = 0;
        for (int i = 0; i < list->size; i++)
            result += list->arr[i];
        return result;
    }
    case PRODUCT:
    {
        uint64_t result = 1;
        for (int i = 0; i < list->size; i++)
            result *= list->arr[i];
        return result;
    }
    case MIN:
    {
        uint64_t result = LONG_MAX;
        for (int i = 0; i < list->size; i++)
            result = result > list->arr[i] ? list->arr[i] : result;
        return result;
    }
    case MAX:
    {
        uint64_t result = 0;
        for (int i = 0; i < list->size; i++)
            result = result < list->arr[i] ? list->arr[i] : result;
        return result;
    }
    case GREATER_THAN:
        return list->arr[0] > list->arr[1];
    case LESS_THAN:
        return list->arr[0] < list->arr[1];
    case EQUAL:
        return list->arr[0] == list->arr[1];
    }

    printf("No matching op id: %d", id);
    return -1;
}

void append(list_t *list, uint64_t val)
{
    if (list->size == list->_alloc_size)
    {
        list->_alloc_size *= 2;
        list->arr = realloc(list->arr, list->_alloc_size * sizeof(*list->arr));
    }

    list->arr[list->size] = val;
    list->size++;
}

uint16_t get_n_next_bits(bitstream_t *stream, uint8_t n)
{
    uint16_t bits = 0;

    for (int i = 0; i < n; i++)
    {
        bits = (bits << 1) | ((stream->arr[stream->arr_offset] >> (sizeof(*stream->arr) * 8 - stream->bit_offset - 1)) & 1);
        stream->bit_offset++;
        stream->arr_offset += stream->bit_offset / (sizeof(*(stream->arr)) * 8);
        stream->bit_offset %= sizeof(*stream->arr) * 8;
    }

    return bits;
}