

void print_right() {
  Serial.print("R: ");
  Serial.print(analogRead(rightEncoder));
  Serial.print("-> ");
  if (readEncoder(rightEncoder) == 1) {
    Serial.println("white");
  }
  else {
    Serial.println("black");
  }
}

void print_left() {
  Serial.print("L: ");
  Serial.print(analogRead(leftEncoder));
  Serial.print("-> ");
  if (readEncoder(leftEncoder) == 1) {
    Serial.println("white");
  }
  else {
    Serial.println("black");
  }
}

void print_wheel_encoders() {
  print_right();
  print_left();
}

void print_inline_encoders() {
  int l = readEncoder(leftEncoder);
  int r = readEncoder(rightEncoder);
  Serial.print("L: ");
  if (l == 1) {
    Serial.print("white ");
  }
  else {
    Serial.print("black ");
  }
  Serial.print(", R: ");
  if (r == 1) {
    Serial.println("white ");
  }
  else {
    Serial.println("black ");
  }
}
