file_contains_recipient_name_list = "pursue_list.txt"
file_contains_email_template = "email_template.txt"

with open(file_contains_recipient_name_list, "r") as pursue_list:
    pursue_list_line = pursue_list.readlines()
    total_line = range(len(pursue_list_line))

    for line_num in total_line:
        if '<docket_number>' in pursue_list_line[line_num]:
            docket_number = pursue_list_line[line_num][(pursue_list_line[line_num].find('[')+1):(pursue_list_line[line_num].find(']'))]
            inventor_names = pursue_list_line[line_num+1][(pursue_list_line[line_num+1].find('[')+1):(pursue_list_line[line_num+1].find(']'))]           
            manager_names = pursue_list_line[line_num+2][(pursue_list_line[line_num+2].find('[')+1):(pursue_list_line[line_num+2].find(']'))]
            primary_inventor = inventor_names.split(",")[0]
            primary_inventor_manager = manager_names.split(",")[0]
            email_filename = "Patent Pursue - %s.txt" %(docket_number)
            
            with open(file_contains_email_template, "r") as email_template:
                with open(email_filename, "w") as file_to_write:
                    email_content_to_write = email_template.read().replace("<docket_number>",docket_number)
                    email_content_to_write = email_content_to_write.replace("<inventor_names>",inventor_names)
                    email_content_to_write = email_content_to_write.replace("<manager_names>",manager_names)
                    email_content_to_write = email_content_to_write.replace("<primary_inventor>",primary_inventor)
                    email_content_to_write = email_content_to_write.replace("<primary_inventor_manager>",primary_inventor_manager)
                    file_to_write.write(email_content_to_write)
                file_to_write.close()
            email_template.close()
pursue_list.close()
