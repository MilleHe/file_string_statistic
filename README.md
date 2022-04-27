# file_string_statistic
- Search the top 20 most frequently used words in the input.
*(tr "[:space:][:punct:]" "\n" | sort | grep -v "^$" | uniq -c | sort -nr | head -20) < ${INPUT_FILE_NAME}*
- make, then run 'bin/main -h' for help.
