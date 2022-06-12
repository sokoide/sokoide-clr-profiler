.text

.p2align    2

.global _EnterNaked
.global _LeaveNaked
.global _TailcallNaked


_EnterNaked:
	stp     x29, x30, [sp, #-16]! // pre-index
	stp     x27, x28, [sp, #-16]!
	stp     x25, x26, [sp, #-16]!
	stp     x23, x24, [sp, #-16]!
	stp     x21, x22, [sp, #-16]!
	stp     x19, x20, [sp, #-16]!
	stp     x17, x18, [sp, #-16]!
	stp     x15, x16, [sp, #-16]!
	stp     x13, x14, [sp, #-16]!
	stp     x11, x12, [sp, #-16]!
	stp     x9, x10, [sp, #-16]!
	stp     x7, x8, [sp, #-16]!
	stp     x5, x6, [sp, #-16]!
	stp     x3, x4, [sp, #-16]!
	stp     x1, x2, [sp, #-16]!
	stp     xzr,x0, [sp, #-16]! // using the dummy xzr register for 16 byte alignment
	bl _EnterStub
	ldp     x1, x0, [sp], #16 // post-index
	ldp     x1, x2, [sp], #16
	ldp     x3, x4, [sp], #16
	ldp     x5, x6, [sp], #16
	ldp     x7, x8, [sp], #16
	ldp     x9, x10, [sp], #16
	ldp     x11, x12, [sp], #16
	ldp     x13, x14, [sp], #16
	ldp     x15, x16, [sp], #16
	ldp     x17, x18, [sp], #16
	ldp     x19, x20, [sp], #16
	ldp     x21, x22, [sp], #16
	ldp     x23, x24, [sp], #16
	ldp     x25, x26, [sp], #16
	ldp     x27, x28, [sp], #16
	ldp     x29, x30, [sp], #16
	ret

_LeaveNaked:
	stp     x29, x30, [sp, #-16]! // pre-index
	stp     x27, x28, [sp, #-16]!
	stp     x25, x26, [sp, #-16]!
	stp     x23, x24, [sp, #-16]!
	stp     x21, x22, [sp, #-16]!
	stp     x19, x20, [sp, #-16]!
	stp     x17, x18, [sp, #-16]!
	stp     x15, x16, [sp, #-16]!
	stp     x13, x14, [sp, #-16]!
	stp     x11, x12, [sp, #-16]!
	stp     x9, x10, [sp, #-16]!
	stp     x7, x8, [sp, #-16]!
	stp     x5, x6, [sp, #-16]!
	stp     x3, x4, [sp, #-16]!
	stp     x1, x2, [sp, #-16]!
	stp     xzr,x0, [sp, #-16]! // using the dummy xzr register for 16 byte alignment
	bl _LeaveStub
	ldp     x1, x0, [sp], #16 // post-index
	ldp     x1, x2, [sp], #16
	ldp     x3, x4, [sp], #16
	ldp     x5, x6, [sp], #16
	ldp     x7, x8, [sp], #16
	ldp     x9, x10, [sp], #16
	ldp     x11, x12, [sp], #16
	ldp     x13, x14, [sp], #16
	ldp     x15, x16, [sp], #16
	ldp     x17, x18, [sp], #16
	ldp     x19, x20, [sp], #16
	ldp     x21, x22, [sp], #16
	ldp     x23, x24, [sp], #16
	ldp     x25, x26, [sp], #16
	ldp     x27, x28, [sp], #16
	ldp     x29, x30, [sp], #16
	ret

_TailcallNaked:
	stp     x29, x30, [sp, #-16]! // pre-index
	stp     x27, x28, [sp, #-16]!
	stp     x25, x26, [sp, #-16]!
	stp     x23, x24, [sp, #-16]!
	stp     x21, x22, [sp, #-16]!
	stp     x19, x20, [sp, #-16]!
	stp     x17, x18, [sp, #-16]!
	stp     x15, x16, [sp, #-16]!
	stp     x13, x14, [sp, #-16]!
	stp     x11, x12, [sp, #-16]!
	stp     x9, x10, [sp, #-16]!
	stp     x7, x8, [sp, #-16]!
	stp     x5, x6, [sp, #-16]!
	stp     x3, x4, [sp, #-16]!
	stp     x1, x2, [sp, #-16]!
	stp     xzr,x0, [sp, #-16]! // using the dummy xzr register for 16 byte alignment
	bl _TailcallStub
	ldp     x1, x0, [sp], #16 // post-index
	ldp     x1, x2, [sp], #16
	ldp     x3, x4, [sp], #16
	ldp     x5, x6, [sp], #16
	ldp     x7, x8, [sp], #16
	ldp     x9, x10, [sp], #16
	ldp     x11, x12, [sp], #16
	ldp     x13, x14, [sp], #16
	ldp     x15, x16, [sp], #16
	ldp     x17, x18, [sp], #16
	ldp     x19, x20, [sp], #16
	ldp     x21, x22, [sp], #16
	ldp     x23, x24, [sp], #16
	ldp     x25, x26, [sp], #16
	ldp     x27, x28, [sp], #16
	ldp     x29, x30, [sp], #16
	ret
