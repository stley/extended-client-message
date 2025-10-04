#tryinclude <Pawn.RakNet>
#if !defined PAWNRAKNET_INC_
    #error "Pawn.RakNet not found (it is necessary to use extended client message!)"
#endif
#tryinclude <PawnPlus>
#if !defined _PawnPlus_included
    #error "PawnPlus not found (it is necessary to use extended client message!)"
#endif

static const rpcSendClientMessage = 93; //SendClientMessage - ID: 93 | Parameters: UINT32 dColor, UINT32 dMessageLength, char[] Message

ORPC:rpcSendClientMessage(playerid, BitStream:bs){
	
	new length;
	new color;
	BS_ReadValue(bs,
	PR_UINT32, color,
	PR_UINT32, length);
	if (length <= 144) return 1;
	new String:message_s = str_new("");
	new bitty;
	for(new l; l < length; l++){
		BS_SetReadOffset(bs, 64+(l*8));
		BS_ReadUint8(bs, bitty);
		str_append_format(message_s, "%c", bitty);
	}
	print_s(message_s);
	new String:substr;
	new buffer[145];
	while(str_len(message_s) > 144){
		new color_embed = str_findc(message_s, '{');
		new color_close = str_findc(message_s, '}');
		new String:embed;
		if(color_embed != -1)
			embed = str_sub(message_s, color_embed, color_close+1);
		substr = str_sub(message_s, 0, ((color_embed) == -1 || color_embed == 0) ? 143 : color_embed);
		print_s(substr);
		str_del(message_s, 0, ((color_embed) == -1 || color_embed == 0) ? 143 : color_embed);
		if(color_embed != -1)
			str_ins(message_s, embed, 0);
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