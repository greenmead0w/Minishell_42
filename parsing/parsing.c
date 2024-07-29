#include "../minishell.h"

/*
split command line into pipe segments
*/
char **split_by_pipe(char *line, int start, int segment_index, int i) 
{
    int segments;
    char **result;

	segments = count_segments(line);
	result = malloc((segments + 1) * sizeof(char *));
	if (result == NULL)
		return NULL;
    while(segment_index < segments) 
	{
        if (line[i] == '\'' || line[i] == '"')
            skip_quotes(line, &i);
        if (line[i] == '|' || line[i] == '\0')
		{
            result[segment_index] = malloc((i - start + 1) * sizeof(char));
			if(result[segment_index] == NULL)
				return NULL;
            ft_strlcpy(result[segment_index], line + start, i - start + 1);
            segment_index++;
            start = i + 1;
        }
		i++;
    }
    result[segments] = NULL;
    return result;
}

/* 
    *initializes a new t_parsed_data node and handles functions to fill the node with data
    *   @param: an array of strings, representing a pipe segment
    *    splitted by spaces.
    *   returns pointer to the head of the linked list
*/
void parse_line(t_parsed_data **header, char **pipe_segment) 
{
    t_parsed_data *node;
	char **cpy_segment;

    node = (t_parsed_data *)malloc(sizeof(t_parsed_data)); // to be freed with list
	if (node == NULL)
		exit(1); //pre_parse_cleanup + free_matrix(split_spaces) + free_matrix(pipe_segments)
	node->path = NULL;
    node->cmd = NULL;
    node->args = NULL;
    node->simple_in_redir = NULL;
    node->simple_out_redir = NULL;
    node->here_doc = NULL;
    node->append = NULL;
    node->next = NULL;
    cpy_segment = parse_redir(node, pipe_segment); //fills node with redirection data
    parse_cmd(node, pipe_segment, cpy_segment); //fills node with command data
    parse_add_node(header, node); //add nodes to the bottom of the list
}

t_parsed_data *parsing(t_sh_data **sh, char *line)
{
	char **pipe_segments;
	int i;
	char **split_space;
	t_parsed_data *head;

	i = 0;
	add_space(sh, line);
	pipe_segments = split_by_pipe((*sh)->new_line, 0, 0, 0);
	if(pipe_segments == NULL)
		exit(1); //pre_parse_cleanup (sh, 0, 0)
    head = NULL;
    while(pipe_segments[i] != NULL)
    {
        printf("pipe_segment %d is: %s\n", i, pipe_segments[i]);
        i++;
    }
    i = 0;
	while(pipe_segments[i] != NULL)
	{
		split_space = ft_split_quotes(pipe_segments[i++], ' ');
        //if (split_space == NULL)
            //pre_parse_cleanup (sh, 0, 0) + free_matrix(pipe_segments);
		parse_line(&head, split_space);
        free_matrix(split_space); // when freeing this I'm also freeing the values in sh, bcause have not done ft_strdup
	}
    free_matrix(pipe_segments); // when freeing this I'm also freeing the values in sh, bcause have not done ft_strdup
    return head;
}

