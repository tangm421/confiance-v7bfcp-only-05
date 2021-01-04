#include <stdio.h>
#include <stdlib.h>
#include "bfcp_messages.h"
#include "bfcp_strings.h"


/* Main Code */
int main(int argc, char *argv[])
{
	int i, j, fatal;
	FILE *f;
	printf("\n*** ChairAction building testcode ***\n\n");
	bfcp_arguments *arguments = bfcp_new_arguments();
	if(!arguments) {
		printf("    - Error in building arguments!\n\n");
		return -1;
	}
	arguments->entity = bfcp_new_entity(1000, 100, 10);
	arguments->primitive = ChairAction;
	bfcp_overall_request_status *oRS = bfcp_new_overall_request_status(11, BFCP_ACCEPTED, 0, "This is an overall request status");
	bfcp_floor_request_status *fRS1 = bfcp_new_floor_request_status(25, BFCP_PENDING, 0, "First Floor Request Status");
	bfcp_floor_request_status *fRS2 = bfcp_new_floor_request_status(9, BFCP_GRANTED, 0, "Floor Request Status n.2");
	bfcp_list_floor_request_status(fRS1, fRS2, NULL);
	bfcp_user_information *beneficiary = bfcp_new_user_information(123, "Oscar Novo", "oscar.novo@mail.com");
	bfcp_user_information *requested_by = bfcp_new_user_information(456, "Lorenzo Miniero", "lminiero@yahoo.com");
	arguments->frqInfo = bfcp_new_floor_request_information(11, oRS, fRS1, beneficiary, requested_by, BFCP_NORMAL_PRIORITY, "Some participant info too");
	if(!arguments->frqInfo) {
		printf("    - Error in building the Floor Request Information!\n");
		bfcp_free_arguments(arguments);
		return -1;
	}
	bfcp_message *message = bfcp_build_message(arguments);
	if(!message) {
		printf("    - Error in building message!\n");
		bfcp_free_arguments(arguments);
		return -1;
	}
	printf("length = %d,  position = %d\n", message->length, message->position);
	f = fopen("mesg.hex", "wb");
	if(!f)
		return -1;
	fwrite(message->buffer, sizeof(char), message->length, f);
	fclose(f);
	arguments->pInfo = NULL;
	bfcp_free_arguments(arguments);
	bfcp_free_message(message);
	printf("\nParsing message...\n");
	f = fopen("mesg.hex", "rb");
	if(!f)
		return -1;
	unsigned char buffer[1024];
	int length = fread(buffer, sizeof(char), 1024, f);
	message = bfcp_new_message(buffer, length);
	fclose(f);
	bfcp_received_message *recvM = bfcp_parse_message(message);
	if(!recvM) {
		printf("Error in parsing message!\n");
		return -1;
	}
	bfcp_received_message_error *tempErr = recvM->errors;
	fatal = 0;
	if(tempErr) {
		printf("---\n");
		printf("There were some errors in parsing:\n");
		j = 0;
		while(tempErr) {
			j++;
			printf("    Error n.%d:    %d (%s) in attribute %d (%s)\n", j, tempErr->code, bfcp_parsing_error[tempErr->code-1].description, tempErr->attribute, bfcp_attribute[tempErr->attribute-1].description);
			if((tempErr->attribute) == 0)
				fatal = 1;
			tempErr = tempErr->next;
		}
		printf("---\n");
	}
	if(fatal) {
		printf("Fatal errors happened in parsing the message,  so it is not valid\n");
		bfcp_free_message(message);
		bfcp_free_received_message(recvM);
		return -1;
	}
	bfcp_supported_list *tempSup;
	bfcp_floor_id_list *tempID;
	bfcp_floor_request_status *tempFRS;
	bfcp_floor_request_information *tempInfo;
	bfcp_received_attribute *temp = recvM->first_attribute;
	printf("  Message Lenght:   %d\n", message->length);
	printf("  Message Position: %d\n", message->position);
	printf("\n**************\n");
	printf("Common Header:\n");
	printf("  Version:              %d\n", recvM->version);
    printf("  Responder Flag:       %d\n", recvM->responder_flag);
    printf("  Fragmentation Flag:   %d\n", recvM->responder_flag);
	printf("  Reserved:             %d\n", recvM->reserved);
	printf("  Primitive:            %d (%s)\n", recvM->primitive, bfcp_primitive[recvM->primitive-1].description);
	printf("  Lenght:               %d\n", recvM->length);
	printf("  ConferenceID:         %ld\n", recvM->entity->conferenceID);
	printf("  TransactionID:        %d\n", recvM->entity->transactionID);
	printf("  UserID                %d\n", recvM->entity->userID);
	temp = recvM->first_attribute;
	if(!temp)
		return 0;
    printf("\n**************\n");
	printf("Attributes:\n");
	i = 0;
	while(temp) {
		i++;
		printf("  Attribute n.%d:\n", i);
		printf("    Type:       %d (%s)\n", temp->type, bfcp_attribute[temp->type-1].description);
		printf("    Mandatory:  %d (%s)\n", temp->mandatory_bit, temp->mandatory_bit ? "YES" : "NO");
		printf("    Lenght:     %d (excluding Common Header)\n", temp->length);
		printf("    Position:   %d\n", temp->position);
		printf("    Valid:      %d (%s)\n", temp->valid, temp->valid ? "YES" : "NO");
		temp = temp->next;
	}
	arguments = recvM->arguments;
    printf("\n**************\n");
	printf("Arguments:\n");
	if(arguments->primitive)
		printf("  Primitive:        %d (%s)\n---\n", arguments->primitive, bfcp_primitive[arguments->primitive-1].description);
	if(arguments->entity) {
		printf("  Entity:\n");
		printf("    ConferenceID:   %ld\n", arguments->entity->conferenceID);
		printf("    TransactionID:  %d\n", arguments->entity->transactionID);
		printf("    UserID          %d\n---\n", arguments->entity->userID);
	}
	if(arguments->fID) {
		printf("  Floor IDs:\n");
		tempID = arguments->fID;
		j = 0;
		while(tempID) {
			j++;
			printf("    Floor ID n.%d: %d\n", j, tempID->ID);
			tempID = tempID->next;
		}
		printf("---\n");
	}
	if(arguments->frqID)
		printf("  Floor Request ID: %d\n---\n", arguments->frqID);
	if(arguments->bID)
		printf("  Beneficiary ID:   %d\n---\n", arguments->bID);
	if(arguments->priority)
		printf("  Priority:         %d (%s)\n---\n", arguments->priority, bfcp_priority[arguments->priority].description);
	if(arguments->frqInfo) {
		tempInfo = arguments->frqInfo;
		while(tempInfo) {
			printf("  Floor Req.Info:\n");
			if(tempInfo->frqID)
				printf("    F.Rq ID:        %d\n", tempInfo->frqID);
			if(tempInfo->oRS) {
				printf("    Overall Request Status:\n");
				printf("      F.Rq ID:      %d\n", tempInfo->oRS->frqID);
				if(tempInfo->oRS->rs) {
					printf("      RS:           %d (%s)\n", tempInfo->oRS->rs->rs, bfcp_status[tempInfo->oRS->rs->rs-1].description);
					printf("      QP:           %d\n", tempInfo->oRS->rs->qp);
				}
				if(tempInfo->oRS->sInfo)
					printf("      Status Info:    %s\n", tempInfo->oRS->sInfo);
			}
			if(tempInfo->fRS) {
				printf("    Floor Request Statuses:\n");
				tempFRS = tempInfo->fRS;
				j = 0;
				while(tempFRS) {
					j++;
					printf("      Floor Request Status n.%d:\n", j);
					printf("        Floor ID:     %d\n", tempFRS->fID);
					if(tempFRS->rs) {
						printf("        RS:           %d (%s)\n", tempFRS->rs->rs, bfcp_status[tempFRS->rs->rs-1].description);
						printf("        QP:           %d\n", tempFRS->rs->qp);
					}
					if(tempFRS->sInfo)
						printf("        Status Info:    %s\n", tempFRS->sInfo);
					tempFRS = tempFRS->next;
				}
			}
			if(tempInfo->beneficiary) {
				printf("    Beneficiary Info:\n");
				if(tempInfo->beneficiary->ID)
					printf("      ID:           %d\n", tempInfo->beneficiary->ID);
				if(tempInfo->beneficiary->display)
					printf("      Display Name: %s\n", tempInfo->beneficiary->display);
				if(tempInfo->beneficiary->uri)
					printf("      User URI:     %s\n", tempInfo->beneficiary->uri);
			}
			if(tempInfo->requested_by) {
				printf("    Requested by Info:\n");
				if(tempInfo->requested_by->ID)
					printf("      ID:           %d\n", tempInfo->requested_by->ID);
				if(tempInfo->requested_by->display)
					printf("      Display Name: %s\n", tempInfo->requested_by->display);
				if(tempInfo->requested_by->uri)
					printf("      User URI:     %s\n", tempInfo->requested_by->uri);
			}
			if(tempInfo->priority)
				printf("    Priority:       %d (%s)\n", tempInfo->priority, bfcp_priority[tempInfo->priority].description);
			if(tempInfo->pInfo)
				printf("    P.P.Info:       %s\n", tempInfo->pInfo);
			printf("---\n");
			tempInfo = tempInfo->next;
		}
	}
	if(arguments->beneficiary) {
		printf("  Beneficiary Info:\n");
		if(arguments->beneficiary->ID)
			printf("    ID:             %d\n", arguments->beneficiary->ID);
		if(arguments->beneficiary->display)
			printf("    Display Name:   %s\n", arguments->beneficiary->display);
		if(arguments->beneficiary->uri)
			printf("    User URI:       %s\n", arguments->beneficiary->uri);
		printf("---\n");
	}
	if(arguments->rs) {
		printf("  Request Status:\n");
		printf("    Request Status: %d (%s)\n", arguments->rs->rs, bfcp_status[arguments->rs->rs-1].description);
		printf("    Queue Position: %d\n", arguments->rs->qp);
		printf("---\n");
	}
	if(arguments->pInfo)
		printf("  P.P.Info:         %s\n---\n", arguments->pInfo);
	if(arguments->sInfo)
		printf("  Status Info:      %s\n---\n", arguments->sInfo);
	if(arguments->error)
		printf("  Error Code:       %d (%s)\n---\n", arguments->error->code, bfcp_error_type[arguments->error->code-1].description);
	if(arguments->eInfo)
		printf("  Error Info:       %s\n---\n", arguments->eInfo);
	if(arguments->primitives) {
		printf("  Supported Primitives:\n");
		tempSup = arguments->primitives;
		while(tempSup) {
			printf("                    %d (%s)\n", tempSup->element, bfcp_primitive[tempSup->element-1].description);
			tempSup = tempSup->next;
		}
		printf("---\n");
	}
	if(arguments->attributes) {
		printf("  Supported Attributes:\n");
		tempSup = arguments->attributes;
		while(tempSup) {
			printf("                    %d (%s)\n", tempSup->element, bfcp_attribute[tempSup->element-1].description);
			tempSup = tempSup->next;
		}
		printf("---\n");
	}
	if(arguments->nonce)
		printf("  Nonce:            %d\n", arguments->nonce);
	if(arguments->digest) {
		printf("  Digest:\n");
		printf("    Algorithm:      %d\n", arguments->digest->algorithm);
		printf("    Digest Text:    %s\n", arguments->digest->text);
		printf("---\n");
	}
	bfcp_free_message(message);
	bfcp_free_received_message(recvM);
	return 0;
}
