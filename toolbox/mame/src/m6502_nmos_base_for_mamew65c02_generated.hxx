void mamew65c02_cpu_device::adc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_adc(m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_adc(m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::adc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::adc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::and_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::and_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asl_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asl_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asl_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asl_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asl_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_asl(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asl_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_asl(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asl_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asl_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asl_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asl_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bcc_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_C)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bcc_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_C)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bcs_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_C) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bcs_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_C) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::beq_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_Z) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::beq_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_Z) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bit_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_bit(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bit_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_bit(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bit_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_bit(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bit_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_bit(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bmi_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_N) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bmi_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_N) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bne_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_Z)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bne_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_Z)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bpl_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_N)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bpl_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_N)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::brk_imp_full()
{
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(m_irq_taken) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	} else {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		m_PC++;
	}
	write(m_SP, m_PC >> 8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	write(m_SP, m_irq_taken ? m_P & ~F_B : m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	dec_SP();
	if(m_nmi_pending) {
		standard_irq_callback(NMI_LINE, m_PC);
		m_PC = read_arg(0xfffa);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
		m_PC = set_h(m_PC, read_arg(0xfffb));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
		m_nmi_pending = false;
	} else {
		if(m_irq_taken)
			standard_irq_callback(IRQ_LINE, m_PC);
		m_PC = read_arg(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
		m_PC = set_h(m_PC, read_arg(0xffff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 17;
		} else
			m_inst_substate = 18;
		return;
	}
	}
	m_irq_taken = false;
	m_P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 19;
		} else
			m_inst_substate = 20;
		return;
	}
	prefetch_end();
	m_inst_state = -1;
}

void mamew65c02_cpu_device::brk_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	// The 6502 bug when a nmi occurs in a brk is reproduced (case !irq_taken && nmi_pending)
	if(m_irq_taken) {
	[[fallthrough]];
case 1:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	} else {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		m_PC++;
	}
	[[fallthrough]];
case 5:
	write(m_SP, m_PC >> 8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	write(m_SP, m_irq_taken ? m_P & ~F_B : m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	dec_SP();
	if(m_nmi_pending) {
		standard_irq_callback(NMI_LINE, m_PC);
	[[fallthrough]];
case 11:
		m_PC = read_arg(0xfffa);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
		m_PC = set_h(m_PC, read_arg(0xfffb));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
		m_nmi_pending = false;
	} else {
		if(m_irq_taken)
			standard_irq_callback(IRQ_LINE, m_PC);
	[[fallthrough]];
case 15:
		m_PC = read_arg(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	[[fallthrough]];
case 17:
		m_PC = set_h(m_PC, read_arg(0xffff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 17;
		} else
			m_inst_substate = 18;
		return;
	}
	[[fallthrough]];
case 18:;
	}
	m_irq_taken = false;
	m_P |= F_I; // Do *not* move after the prefetch
	prefetch_start();
	[[fallthrough]];
case 19:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 19;
		} else
			m_inst_substate = 20;
		return;
	}
	[[fallthrough]];
case 20:;
	prefetch_end();
	m_inst_state = -1;
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bvc_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(!(m_P & F_V)) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bvc_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(!(m_P & F_V)) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::bvs_rel_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	if(m_P & F_V) {
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
		if(page_changing(m_PC, int8_t(m_TMP))) {
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::bvs_rel_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	if(m_P & F_V) {
	[[fallthrough]];
case 3:
		read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
		if(page_changing(m_PC, int8_t(m_TMP))) {
	[[fallthrough]];
case 5:
			read_arg(set_l(m_PC, m_PC+int8_t(m_TMP)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
		}
		m_PC += int8_t(m_TMP);
	}
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::clc_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::clc_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cld_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_D;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cld_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cli_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
	m_P &= ~F_I; // Do *not* move it before the prefetch
}

void mamew65c02_cpu_device::cli_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	m_P &= ~F_I; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::clv_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P &= ~F_V;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::clv_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P &= ~F_V;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_A, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_A, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cmp_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cmp_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpx_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_X, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_X, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_X, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpy_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpy_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpy_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_cmp(m_Y, m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpy_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_cmp(m_Y, m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::cpy_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::cpy_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_cmp(m_Y, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dec_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dec_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dec_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dec_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dec_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dec_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dec_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dec_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dex_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X--;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dex_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X--;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dey_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y--;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dey_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y--;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A ^= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A ^= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::eor_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::eor_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::inc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::inc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::inc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::inc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::inc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::inc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::inc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	set_nz(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::inc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	set_nz(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::inx_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X++;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::inx_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X++;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::iny_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y++;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::iny_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y++;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::jmp_adr_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC = m_TMP;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::jmp_adr_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC = m_TMP;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::jmp_ind_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_PC = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_PC = set_h(m_PC, read(set_l(m_TMP, m_TMP+1)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::jmp_ind_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_PC = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_PC = set_h(m_PC, read(set_l(m_TMP, m_TMP+1)));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::jsr_adr_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_SP, m_PC>>8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_PC = m_TMP;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::jsr_adr_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_SP, m_PC>>8);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	write(m_SP, m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_PC = m_TMP;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	m_A = read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	m_A = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_A = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_A = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_imm_full()
{
	m_A = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_A = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_A = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lda_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lda_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_A = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldx_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_X = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldx_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	m_X = read(m_TMP + m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldx_imm_full()
{
	m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldx_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_X = read(uint8_t(m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldx_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_X = read(uint8_t(m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldy_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldy_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldy_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldy_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldy_imm_full()
{
	m_Y = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldy_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_Y = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldy_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldy_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_Y = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ldy_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_Y = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ldy_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_Y = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lsr_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lsr_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lsr_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lsr_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lsr_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_lsr(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lsr_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_lsr(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lsr_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lsr_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lsr_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lsr_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A |= m_TMP;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A |= m_TMP;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ora_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ora_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::pha_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	write(m_SP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	dec_SP();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::pha_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	write(m_SP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::php_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	write(m_SP, m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	dec_SP();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::php_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	write(m_SP, m_P);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	dec_SP();
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::pla_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_A = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::pla_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_A = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::plp_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_TMP = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
	m_P = m_TMP; // Do *not* move it before the prefetch
}

void mamew65c02_cpu_device::plp_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_TMP = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	m_P = m_TMP; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rol_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rol_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rol_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rol_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rol_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_rol(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rol_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_rol(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rol_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rol_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rol_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rol_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ror_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ror_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ror_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ror_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ror_acc_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = do_ror(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ror_acc_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = do_ror(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ror_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ror_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ror_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ror_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rti_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_P = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	inc_SP();
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	inc_SP();
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rti_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_P = read(m_SP) | (F_B|F_E);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	inc_SP();
	[[fallthrough]];
case 9:
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rts_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	inc_SP();
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	inc_SP();
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_PC++;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rts_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	inc_SP();
	[[fallthrough]];
case 5:
	m_PC = read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	inc_SP();
	[[fallthrough]];
case 7:
	m_PC = set_h(m_PC, read(m_SP));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_PC++;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	do_sbc(m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	do_sbc(m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbc_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbc_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP2 = read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sec_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P |= F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sec_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P |= F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sed_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_P |= F_D;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sed_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_P |= F_D;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sei_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
	m_P |= F_I; // Do *not* move it before the prefetch
}

void mamew65c02_cpu_device::sei_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	m_P |= F_I; // Do *not* move it before the prefetch
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP+m_X, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP+m_X, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP+m_Y, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sta_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_X), m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sta_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(m_TMP+m_X), m_A);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::stx_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::stx_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::stx_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::stx_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::stx_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_Y), m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::stx_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(m_TMP+m_Y), m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sty_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sty_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sty_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sty_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	write(m_TMP, m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sty_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(uint8_t(m_TMP+m_X), m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sty_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(uint8_t(m_TMP+m_X), m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::tax_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X = m_A;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::tax_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X = m_A;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::tay_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_Y = m_A;
	set_nz(m_Y);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::tay_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_Y = m_A;
	set_nz(m_Y);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::tsx_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_X = m_SP;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::tsx_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_X = m_SP;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::txa_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = m_X;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::txa_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = m_X;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::txs_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_SP = set_l(m_SP, m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::txs_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_SP = set_l(m_SP, m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::tya_imp_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_A = m_Y;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::tya_imp_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_A = m_Y;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::reset_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	dec_SP();
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	dec_SP();
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	dec_SP();
	m_P |= F_I;
	m_PC = read_arg(0xfffc);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_PC = set_h(m_PC, read_arg(0xfffd));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
	m_inst_state = -1;
}

void mamew65c02_cpu_device::reset_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	[[fallthrough]];
case 3:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	dec_SP();
	[[fallthrough]];
case 7:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	dec_SP();
	[[fallthrough]];
case 9:
	read(m_SP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	dec_SP();
	m_P |= F_I;
	[[fallthrough]];
case 11:
	m_PC = read_arg(0xfffc);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	[[fallthrough]];
case 13:
	m_PC = set_h(m_PC, read_arg(0xfffd));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	m_inst_state = -1;
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2--;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2--;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2--;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2--;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::dcp_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2--;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::dcp_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2--;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_cmp(m_A, m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2++;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2++;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2++;
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2++;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::isb_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2++;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_sbc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::isb_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2++;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_sbc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	}
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 7:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	}
	[[fallthrough]];
case 9:
	m_A = m_X = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lax_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_Y);
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lax_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_Y);
	[[fallthrough]];
case 5:
	m_A = m_X = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rla_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_rol(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rla_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_rol(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A &= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::rra_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_ror(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	do_adc(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::rra_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_ror(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	do_adc(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sax_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sax_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sax_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sax_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sax_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sax_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 3:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sax_zpy_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_Y);
	m_TMP2 = m_A & m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sax_zpy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_Y);
	m_TMP2 = m_A & m_X;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sbx_imm_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_X &= m_A;
	if(m_X < m_TMP2)
		m_P &= ~F_C;
	else
		m_P |= F_C;
	m_X -= m_TMP2;
	set_nz(m_X);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sbx_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_X &= m_A;
	if(m_X < m_TMP2)
		m_P &= ~F_C;
	else
		m_P |= F_C;
	m_X -= m_TMP2;
	set_nz(m_X);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sha_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sha_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sha_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sha_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::shs_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_SP = set_l(m_SP, m_A & m_X);
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::shs_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_SP = set_l(m_SP, m_A & m_X);
	m_TMP2 = m_A & m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::shx_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::shx_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_X & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_Y))
		m_TMP = set_h(m_TMP+m_Y, m_TMP2);
	else
		m_TMP += m_Y;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::shy_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = m_Y & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_X))
		m_TMP = set_h(m_TMP+m_X, m_TMP2);
	else
		m_TMP += m_X;
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::shy_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = m_Y & ((m_TMP >> 8)+1);
	if(page_changing(m_TMP, m_X))
		m_TMP = set_h(m_TMP+m_X, m_TMP2);
	else
		m_TMP += m_X;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::slo_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_asl(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::slo_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_asl(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A |= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_X;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_X;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP += m_Y;
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 13:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_idx_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP2 += m_X;
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_idx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP2 += m_X;
	[[fallthrough]];
case 5:
	m_TMP = read(m_TMP2 & 0xff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_idy_full()
{
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP += m_Y;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_idy_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP2 = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = read(m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	m_TMP = set_h(m_TMP, read((m_TMP2+1) & 0xff));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP += m_Y;
	[[fallthrough]];
case 9:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	[[fallthrough]];
case 11:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 13:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 13;
		} else
			m_inst_substate = 14;
		return;
	}
	[[fallthrough]];
case 14:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 15:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 15;
		} else
			m_inst_substate = 16;
		return;
	}
	[[fallthrough]];
case 16:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::sre_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_TMP = uint8_t(m_TMP+m_X);
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_TMP2 = do_lsr(m_TMP2);
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::sre_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_TMP = uint8_t(m_TMP+m_X);
	[[fallthrough]];
case 5:
	m_TMP2 = read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_TMP2 = do_lsr(m_TMP2);
	[[fallthrough]];
case 9:
	write(m_TMP, m_TMP2);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	m_A ^= m_TMP2;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 11:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 11;
		} else
			m_inst_substate = 12;
		return;
	}
	[[fallthrough]];
case 12:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::anc_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	if(m_A & 0x80)
		m_P |= F_C;
	else
		m_P &= ~F_C;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::anc_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	set_nz(m_A);
	if(m_A & 0x80)
		m_P |= F_C;
	else
		m_P &= ~F_C;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::ane_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP & m_X;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::ane_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP & m_X;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::asr_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A = do_lsr(m_A & m_TMP);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::asr_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A = do_lsr(m_A & m_TMP);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::arr_imm_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_A &= m_TMP;
	do_arr();
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::arr_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	m_A &= m_TMP;
	do_arr();
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::las_aby_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	m_A = m_TMP2 | 0x51;
	m_X = 0xff;
	set_nz(m_TMP2);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::las_aby_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_Y)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_Y));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	m_TMP2 = read(m_TMP+m_Y);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	m_A = m_TMP2 | 0x51;
	m_X = 0xff;
	set_nz(m_TMP2);
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::lxa_imm_full()
{
	m_A = m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::lxa_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_A = m_X = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	set_nz(m_A);
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_aba_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_aba_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	[[fallthrough]];
case 5:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_abx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	}
	read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_abx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	m_TMP = set_h(m_TMP, read_pc());
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	m_PC++;
	if(page_changing(m_TMP, m_X)) {
	[[fallthrough]];
case 5:
		read(set_l(m_TMP, m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	}
	[[fallthrough]];
case 7:
	read(m_TMP + m_X);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_start();
	[[fallthrough]];
case 9:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_imm_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_imm_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	prefetch_start();
	[[fallthrough]];
case 3:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_zpg_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_zpg_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	prefetch_start();
	[[fallthrough]];
case 5:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::nop_zpx_full()
{
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	prefetch_start();
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	prefetch_end();
}

void mamew65c02_cpu_device::nop_zpx_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	m_TMP = read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(m_TMP);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(uint8_t(m_TMP+m_X));
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	prefetch_start();
	[[fallthrough]];
case 7:
	m_IR = read_sync(m_PC);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	prefetch_end();
	break;
}
	m_inst_substate = 0;
}

void mamew65c02_cpu_device::kil_non_full()
{
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	m_PC++;
	read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	for(;;) {
		read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	}
}

void mamew65c02_cpu_device::kil_non_partial()
{
	switch(m_inst_substate) {
case 0:
	[[fallthrough]];
case 1:
	read_pc();
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 1;
		} else
			m_inst_substate = 2;
		return;
	}
	[[fallthrough]];
case 2:;
	m_PC++;
	[[fallthrough]];
case 3:
	read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 3;
		} else
			m_inst_substate = 4;
		return;
	}
	[[fallthrough]];
case 4:;
	[[fallthrough]];
case 5:
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 5;
		} else
			m_inst_substate = 6;
		return;
	}
	[[fallthrough]];
case 6:;
	[[fallthrough]];
case 7:
	read(0xfffe);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 7;
		} else
			m_inst_substate = 8;
		return;
	}
	[[fallthrough]];
case 8:;
	for(;;) {
	[[fallthrough]];
case 9:
		read(0xffff);
	m_icount--;
	if(m_icount <= 0) {
		if(access_to_be_redone()) {
			m_icount++;
			m_inst_substate = 9;
		} else
			m_inst_substate = 10;
		return;
	}
	[[fallthrough]];
case 10:;
	}
	break;
}
	m_inst_substate = 0;
}





