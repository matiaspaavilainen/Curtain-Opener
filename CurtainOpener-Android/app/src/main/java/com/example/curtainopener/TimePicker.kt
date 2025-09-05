package com.example.curtainopener

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.DropdownMenu
import androidx.compose.material3.DropdownMenuItem
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.material3.TimePicker
import androidx.compose.material3.TimePickerState
import androidx.compose.material3.rememberTimePickerState
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.ui.window.Dialog
import java.time.DayOfWeek
import java.time.LocalDateTime

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun TimeDialog(
    onDismissRequest: () -> Unit,
    onConfirmation: (TimePickerState, MutableList<String>) -> Unit,
    openTimeDialog: MutableState<Boolean>,
    currentDay: String,
    currentTime: LocalDateTime
) {
    val timePickerState = rememberTimePickerState(
        initialHour = currentTime.hour,
        initialMinute = currentTime.minute,
        is24Hour = true,
    )

    val daysToChange: MutableList<String> = mutableListOf(currentDay)

    Dialog(onDismissRequest = { openTimeDialog.value = false }) {
        Card(
            modifier = Modifier
                .fillMaxWidth()
                .height(600.dp)
                .padding(8.dp),
            shape = RoundedCornerShape(8.dp),
        ) {
            Column(
                modifier = Modifier.fillMaxSize(),
                verticalArrangement = Arrangement.Center,
                horizontalAlignment = Alignment.CenterHorizontally,
            ) {
                TimePicker(
                    state = timePickerState,
                )

                DropDown(currentDay, daysToChange)

                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.Center,
                ) {
                    TextButton(
                        onClick = { onDismissRequest() },
                        modifier = Modifier.padding(8.dp),
                    ) {
                        Text("Dismiss")
                    }
                    TextButton(
                        onClick = { onConfirmation(timePickerState, daysToChange) },
                        modifier = Modifier.padding(8.dp),
                    ) {
                        Text("Confirm")
                    }
                }
            }
        }
    }
}

@Composable
fun DropDown(currentDay: String, daysToChange: MutableList<String>) {
    var expanded by remember { mutableStateOf(false) }
    val dropDownList: List<String> = listOf(currentDay, "Weekdays", "Weekend", "All")
    val daysToChangeList: List<List<String>> = listOf(
        listOf(currentDay),
        listOf("Monday", "Tuesday", "Wednesday", "Thursday", "Friday"),
        listOf("Saturday", "Sunday"),
        listOf("Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday")
    )
    var index by remember { mutableIntStateOf(0) }
    Box(
        modifier = Modifier
            .padding(16.dp)
    ) {
        Text(
            text = dropDownList[index],
            modifier = Modifier
                .clickable(true, onClick = {
                    expanded = !expanded
                })
        )
        DropdownMenu(
            expanded = expanded,
            onDismissRequest = { expanded = false }
        ) {
            dropDownList.forEach { day ->
                DropdownMenuItem(
                    text = { Text(text = day) },
                    onClick = {
                        index = dropDownList.indexOf(day)
                        daysToChange.clear()
                        daysToChange.addAll(0, daysToChangeList[index])
                        println("Days to change $daysToChange")
                        expanded = false
                    }
                )
            }
        }
    }
}