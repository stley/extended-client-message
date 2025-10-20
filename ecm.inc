#if defined _inc_extended_client_message
    #endinput
#endif
#define _inc_extended_client_message

#tryinclude <Pawn.RakNet>
#if !defined PAWNRAKNET_INC_
    #error "Pawn.RakNet not found (it is necessary to use extended client message!)"
#endif
#tryinclude <PawnPlus>
#if !defined _PawnPlus_included
    #error "PawnPlus not found (it is necessary to use extended client message!)"
#endif

stock ecm_ReadString(BitStream:bs, String:output, offset, length){
    new bitty;
    for(new l; l < length; l++){
        BS_SetReadOffset(bs, offset+(l*8));
        BS_ReadUint8(bs, bitty);
        str_append_format(output, "%c", bitty);
    }
}
static const rpcSendClientMessage = 93; //SendClientMessage - ID: 93 | Parameters: UINT32 dColor, UINT32 dMessageLength, char[] Message

stock str_rfindc(ConstStringTag:string, value){
	for(new i = str_len(string)-1; i >= 0; i--){
		if(str_getc(string, i) == value) return i;
	}
	return -1;
}

ORPC:rpcSendClientMessage(playerid, BitStream:bs){
	
	new length;
	new color;
	BS_ReadValue(bs,
	PR_UINT32, color,
	PR_UINT32, length);
	if (length <= 144) return 1;
	new String:message_s = str_new("");
    ecm_ReadString(bs, message_s, 64, length);
	new String:substr;
	new buffer[145];
	while(str_len(message_s) > 144){
		substr = str_sub(message_s, 0, 144);
		str_del(message_s, 0, 144);
		if(str_len(message_s) > 144){
			if(str_len(substr) >= 144 && str_getc(substr, 143) != ' ' && str_getc(message_s, 0) != ' '){
				new spit_pos = str_rfindc(substr, ' ');
				if(spit_pos != -1 && spit_pos < str_len(substr)){
					str_ins(message_s, str_sub(substr, spit_pos+1, str_len(substr)), 0);
					str_del(substr, spit_pos+1);
				}
			}
		}
		new color_embed = str_rfindc(substr, '{');
		new color_close = str_rfindc(substr, '}');
		new String:embed;
		if(color_embed != -1 && color_close != -1)
			embed = str_sub(substr, color_embed, color_close+1);
		if(str_valid(embed) && str_len(embed) == 8){
			str_ins(message_s, embed, 0);
			str_delete(embed);
		}
		str_get(substr, buffer);
		SendClientMessage(playerid, color, buffer);
		str_delete(substr);
	}
	if(str_len(message_s)){
		str_get(message_s, buffer);
		SendClientMessage(playerid, color, buffer);
		str_delete(message_s);
	}
	return 0;
}
