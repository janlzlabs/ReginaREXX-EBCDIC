/* sample routines to convert dates                                   */

  call Charout , "Enter a date in the format dd.mm.yyyy: "
  curDMYDate = lineIn()

  curJulianDate = DMYToJulian( curDMYDate )
  say "DMYToJulian(" || curDMYDate || ") is " || curJulianDate

  say "JulianToDMY(" || curJulianDate || ") is " || ,
      JulianToDMY( curJulianDate )

exit 0

/* ------------------------------------------------------------------ */
/* function: Convert a date from Julian to DMY                        */
/*                                                                    */
/* call:     JulianToDMY julianDate {trans}                           */
/*                                                                    */
/* where:    julianDate - the date in julian format                   */
/*           trans - see note above                                   */
/*                                                                    */
/* returns:  the date in the format dd.mm.yyyy                        */
/*                                                                    */
/*                                                                    */
JulianToDMY: PROCEDURE
  Arg J Trans

  if Trans = "" then
    Trans = 2299161

  if J < Trans then
    A = J
  Else
  do
    AA = J - 1721120
    AC = Trunc(AA / 1460969)
    AB = 31 * AC
    AA = AA - AC * 1460969
    AC = Trunc(AA / 146097)
    AB = AB + 3 * AC
    AA = AA - AC * 146097
    if AA = 146096 then
      AB = AB + 3
    Else
      AB = AB + Trunc(AA / 36524)
    A = J + (AB - 2)
  end
  B = A + 1524
  C = Trunc((20 * B - 2442) / 7305)
  D = Trunc(1461 * C / 4)
  EE = B - D
  E = Trunc(10000 * EE / 306001)
  YMDD = EE - Trunc(306001 * E / 10000)
  if E >= 14 then
    YMDM = E - 13
  else
    YMDM = E - 1
  if YMDM > 2 then
    Y = C - 4716
  else
    Y = C - 4715
  if Y < 1 then
    YMDY = Y - 1
  else
    YMDY = Y

RETURN YMDD || '.' || YMDM || '.' || YMDY
/* ------------------------------------------------------------------ */
/* function: Convert a date from DMY to Julian                        */
/*                                                                    */
/* call:     DMYToJulian dmyDate {trans}                              */
/*           trans - see note above                                   */
/*                                                                    */
/* where:    dmyDate - the date in the format dd.mm.yyyy              */
/*                                                                    */
/* returns:  the date in Julian format                                */
/*                                                                    */
/*                                                                    */
DMYToJulian: PROCEDURE
  parse arg dmyDate trans

  parse var dmyDate YMDD "." YMDM "." YMDY

  if Trans = "" then
    Trans = 2299161

  AY = YMDY
  if YMDY < 0 then
    Y = YMDY + 4717
  else
    Y = YMDY + 4716
  if YMDM < 3 then
  do
    M = YMDM + 12
    Y = Y - 1
    AY = AY - 1
  end
  else
    M = YMDM
  D = Trunc((1461 * Y) / 4) + Trunc((153 * (M + 1)) / 5) + YMDD - 1524
  G = D + 2 - Trunc(AY / 100) + Trunc(AY / 400) - Trunc(AY / 4000)
  if G >= Trans then
    thisRC = G
  else
    thisRC = D

RETURN thisRC
