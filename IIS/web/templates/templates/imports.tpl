{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	<form action="{$U_IMPORT}" method="post" enctype="multipart/form-data">
		<table width="100%" border="0">
			<tr>
				<td class="table-label">{$L_IMPORT_FORMAT}:<span class="mandatory-field">*</span></td>
				<td class="table-input">
					<select name="import_format">
						{foreach item=format from=$FORMATS}
							<option value="{$format.format_id}" {if $SELECTED_FORMAT eq $format.format_id} selected="selected"{/if}>{$format.name}</option>
						{/foreach}
					</select>
				</td>
			</tr>
			<tr>
				<td class="table-label">{$L_FILE_WITH_IMPORT_DATA}:<span class="mandatory-field">*</span></td>
				<td class="table-input"><input type="file" name="file_to_import" size="24" /></td>
			</tr>
		</table>

		<div class="action-buttons">
			<input class="submit" type="submit" name="import" value="{$L_DO_IMPORT}" />
		</div>

		<div class="align-left">
			<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
		</div>
	</form>
{include file='module_footer.tpl'}
