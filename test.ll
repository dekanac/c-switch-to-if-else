; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %eqtmp = icmp eq i32 %i1, 4
  %booltmp3 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp3, 0.000000e+00
  %c4 = load double, double* %c
  br i1 %ifcond, label %then, label %ifcont

afterloop:                                        ; preds = %loop1
  %c9 = load double, double* %c
  ret double %c9

then:                                             ; preds = %loop2
  %subtmp = fsub double %c4, 1.000000e+02
  store double %subtmp, double* %c
  br label %ifcont

ifcont:                                           ; preds = %loop2, %then
  %c6 = load double, double* %c
  %addtmp = fadd double %c6, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp8 = add i32 %i1, 1
  store i32 %addtmp8, i32* %i
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %eqtmp = icmp eq i32 %i1, 4
  %booltmp3 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp3, 0.000000e+00
  %c4 = load double, double* %c
  br i1 %ifcond, label %then, label %ifcont

afterloop:                                        ; preds = %loop1
  %c9 = load double, double* %c
  ret double %c9

then:                                             ; preds = %loop2
  %subtmp = fsub double %c4, 1.000000e+02
  store double %subtmp, double* %c
  br label %ifcont

ifcont:                                           ; preds = %loop2, %then
  %c6 = load double, double* %c
  %addtmp = fadd double %c6, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp8 = add i32 %i1, 1
  store i32 %addtmp8, i32* %i
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  %c2 = load double, double* %c
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  %eqtmp = icmp eq i32 %addtmp4, 4
  %booltmp6 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp6, 0.000000e+00
  br i1 %ifcond, label %then, label %else

afterloop:                                        ; preds = %loop1
  store double %c2, double* %r
  ret double %c2

then:                                             ; preds = %loop2
  %subtmp = fsub double %addtmp, 1.000000e+00
  store double %subtmp, double* %c
  br label %ifcont

else:                                             ; preds = %loop2
  %addtmp9 = fadd double %addtmp, 1.000000e+00
  store double %addtmp9, double* %c
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %c2 = load double, double* %c
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  %eqtmp = icmp eq i32 %addtmp4, 4
  %booltmp6 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp6, 0.000000e+00
  br i1 %ifcond, label %then, label %else

afterloop:                                        ; preds = %loop1
  store double 3.000000e+00, double* %r
  %c11 = load double, double* %c
  %addtmp12 = fadd double 3.000000e+00, %c11
  store double %addtmp12, double* %r
  ret double %addtmp12

then:                                             ; preds = %loop2
  %subtmp = fsub double %addtmp, 1.000000e+00
  store double %subtmp, double* %c
  br label %ifcont

else:                                             ; preds = %loop2
  %addtmp9 = fadd double %addtmp, 1.000000e+00
  store double %addtmp9, double* %c
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %c2 = load double, double* %c
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  %eqtmp = icmp eq i32 %addtmp4, 4
  %booltmp6 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp6, 0.000000e+00
  br i1 %ifcond, label %then, label %else

afterloop:                                        ; preds = %loop1
  store double 3.000000e+00, double* %r
  %c11 = load double, double* %c
  %addtmp12 = fadd double 3.000000e+00, %c11
  store double %addtmp12, double* %r
  ret double %addtmp12

then:                                             ; preds = %loop2
  %subtmp = fsub double %addtmp, 1.000000e+00
  store double %subtmp, double* %c
  br label %ifcont

else:                                             ; preds = %loop2
  %addtmp9 = fadd double %addtmp, 1.000000e+00
  store double %addtmp9, double* %c
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define double @func() {
entry:
  %p = alloca double
  %r = alloca double
  %c = alloca double
  %i = alloca i32
  store i32 0, i32* %i
  store i32 1, i32* %i
  store double 0.000000e+00, double* %c
  store double 0.000000e+00, double* %r
  store double 0.000000e+00, double* %p
  store double 3.200000e+00, double* %c
  br label %loop1

loop1:                                            ; preds = %ifcont, %entry
  %i1 = load i32, i32* %i
  %letmp = icmp ule i32 %i1, 7
  %booltmp = uitofp i1 %letmp to double
  %whilecond = fcmp one double %booltmp, 0.000000e+00
  br i1 %whilecond, label %loop2, label %afterloop

loop2:                                            ; preds = %loop1
  %c2 = load double, double* %c
  %addtmp = fadd double %c2, 1.000000e+00
  store double %addtmp, double* %c
  %addtmp4 = add i32 %i1, 1
  store i32 %addtmp4, i32* %i
  %eqtmp = icmp eq i32 %addtmp4, 4
  %booltmp6 = uitofp i1 %eqtmp to double
  %ifcond = fcmp one double %booltmp6, 0.000000e+00
  br i1 %ifcond, label %then, label %else

afterloop:                                        ; preds = %loop1
  store double 3.000000e+00, double* %r
  %c11 = load double, double* %c
  %addtmp12 = fadd double 3.000000e+00, %c11
  store double %addtmp12, double* %r
  ret double %addtmp12

then:                                             ; preds = %loop2
  %subtmp = fsub double %addtmp, 1.000000e+00
  store double %subtmp, double* %c
  br label %ifcont

else:                                             ; preds = %loop2
  %addtmp9 = fadd double %addtmp, 1.000000e+00
  store double %addtmp9, double* %c
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  br label %loop1
}
; ModuleID = 'swi2else'
source_filename = "swi2else"

define i32 @main() {
entry:
  %c = alloca i32
  store i32 0, i32* %c
  store i32 0, i32* %c
  br i1 false, label %then, label %else

then:                                             ; preds = %entry
  store i32 1, i32* %c
  br label %ifcont

else:                                             ; preds = %entry
  br i1 true, label %then1, label %else2

then1:                                            ; preds = %else
  store i32 2, i32* %c
  br label %ifcont

else2:                                            ; preds = %else
  br i1 false, label %then3, label %else4

then3:                                            ; preds = %else2
  store i32 3, i32* %c
  br label %ifcont

else4:                                            ; preds = %else2
  br i1 true, label %then5, label %else6

then5:                                            ; preds = %else4
  store i32 99, i32* %c
  br label %ifcont

else6:                                            ; preds = %else4
  br label %ifcont

ifcont:                                           ; preds = %else6, %then5, %then3, %then1, %then
  %c7 = load i32, i32* %c
  ret i32 %c7
}
