{*
	Encoding: utf8
	Author:   Petr Zemek, http://www.stud.fit.vutbr.cz/~xzemek02/
*}

{include file='module_header.tpl'}
	<form action="{$U_EXPORT}" method="post" enctype="multipart/form-data">
		<table width="100%" border="0">
			<tr>
				<td class="table-label">{$L_EXPORT_FORMAT}:<span class="mandatory-field">*</span></td>
				<td class="table-input">
					<select name="export_format">
						{foreach item=format from=$FORMATS}
							<option value="{$format.format_id}" {if $SELECTED_FORMAT eq $format.format_id} selected="selected"{/if}>{$format.name}</option>
						{/foreach}
					</select>
				</td>
			</tr>
			<tr>
				<td class="table-label">{$L_TABLE}:<span class="mandatory-field">*</span></td>
				<td class="table-input">
					<select name="table">
						{foreach item=table_name from=$TABLES}
							<option value="{$table_name}"  {if $SELECTED_TABLE eq $table_name} selected="selected"{/if}>{$table_name}</option>
						{/foreach}
					</select>
				</td>
			</tr>
		</table>

		<div class="action-buttons">
			<input class="submit" type="submit" name="export" value="{$L_DO_EXPORT}" />
		</div>

		<div class="align-left">
			<span class="mandatory-field">*</span> - {$L_MANDATORY_FIELDS}
		</div>
	</form>
{include file='module_footer.tpl'}
